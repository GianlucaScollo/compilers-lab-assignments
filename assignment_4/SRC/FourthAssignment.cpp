#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/ScalarEvolutionExpressions.h"
#include "llvm/Analysis/ValueTracking.h" // getUnderlyingObject

using namespace llvm;

//-----------------------------------------------------------------------------
// FourthAssignment implementation
//-----------------------------------------------------------------------------

// Struttura dati per salvare i dati relativi a due loop adiacenti
struct LoopPair {
  Loop* L1 = nullptr;
  Loop* L2 = nullptr;
  bool areGuarded = false;
};

namespace {

  // Funzione helper usata per controllare se i blocchi di guardia/preheader dei due loop sono vuoti
  bool isBlockSafeForFusion(BasicBlock *BB) {
    if (!BB) return false;

    for (Instruction &I : *BB) {
        // Ignoriamo le istruzioni innocue e necessarie per il CFG
        if (I.isTerminator() || isa<PHINode>(&I) || isa<DbgInfoIntrinsic>(&I))
            continue;
            
        // Permettiamo le comparazioni (necessarie se il blocco è una guardia)
        if (isa<CmpInst>(&I))
            continue;

        // Se troviamo qualsiasi altra cosa (add, store, call, load), 
        // c'è della logica di mezzo. Il blocco NON è sicuro/vuoto.
        return false;
    }
    return true;
  }

  // CONDIZIONE 1
  // Controllo se i due loop sono adiacenti tra di loro
  bool areLoopsAdjacent(Loop *L1, Loop *L2, bool areGuarded) {
    BasicBlock *ExitL1 = L1->getUniqueExitBlock();

    // Controllo se L1 e L2 hanno più punti di uscita.
    if (!ExitL1 || !L2->getUniqueExitBlock()) return false;

    BasicBlock *PreheaderLoop2 = L2->getLoopPreheader();

    // Controllo il preheader di L2, verifico che esista e che sia "vuoto"
    if (!PreheaderLoop2 || !isBlockSafeForFusion(PreheaderLoop2)){
      outs() << "Errore nell'ottenimento del Preheader di L2 o il Preheader non è safe\n";
      return false;
    } 

    // Controllo se sono dei loop guarded
    if (areGuarded) {
      BranchInst *GuardBranchL2 = L2->getLoopGuardBranch();
      if (!GuardBranchL2) return false; 
      
      BasicBlock *GuardedBlockLoop2 = GuardBranchL2->getParent();

      // Controllo che anche la guardia di L2 sia vuota
      if (!isBlockSafeForFusion(GuardedBlockLoop2)){
        outs() << "Errore il GuardedBlock2 non è safe\n";
        return false;
      }

      // Verifico che l'uscita di L1 sbatta contro la guardia di L2
      return (ExitL1->getSingleSuccessor() == GuardedBlockLoop2);
    }

    // Se non sono guarded, verifico che l'uscita di L1 sbatta contro il preheader di L2
    return (ExitL1 == PreheaderLoop2);
  }

  // CONDIZIONE 2
  // Verifico che il numero di iterazioni dei due loop sia lo stesso
  // (gestisce anche quando le condizioni di terminazione sono contenute dentro a variabili)
  bool haveSameIterationNumber(Loop *L1, Loop *L2, ScalarEvolution &SE) {
    // 1. Chiediamo l'espressione matematica del numero di iterazioni (Trip Count)
    const SCEV *TC1 = SE.getBackedgeTakenCount(L1);
    const SCEV *TC2 = SE.getBackedgeTakenCount(L2);

    // 2. Se LLVM non riesce a risolverla (es. loop con break complessi)
    if (isa<SCEVCouldNotCompute>(TC1) || isa<SCEVCouldNotCompute>(TC2)) {
        return false;
    }

    // 3. Verifichiamo se le due espressioni matematiche sono identiche.
    // Questo gestisce sia i numeri ("100" == "100") 
    // sia i simboli variabili ("N - 1" == "N - 1")
    return TC1 == TC2;
  }

  // CONDIZIONE 3
  // Verifica Control Flow Equivalence
  bool isControlFlowEquivalent(Loop *L1, Loop *L2, DominatorTree &DT, PostDominatorTree &PDT) {
    // L1 deve dominare L2 (cioè per arrivare a L2 devo sempre passare per L1))
    bool dominates = DT.dominates(L1->getHeader(), L2->getHeader());
    
    // L2 deve POST-dominare L1 (cioè dopo il loop L1 devo sempre passare dal loop L2)
    bool postDominates = PDT.dominates(L2->getHeader(), L1->getHeader());
    
    bool headerCFE = dominates && postDominates;

    // Caso in cui i due loop sono guarded
    if(L1->getLoopGuardBranch() != nullptr && L2->getLoopGuardBranch() != nullptr){
      bool Gdominates = DT.dominates(L1->getLoopGuardBranch()->getParent(), L2->getLoopGuardBranch()->getParent());
      bool GpostDominates = PDT.dominates(L2->getLoopGuardBranch()->getParent(), L1->getLoopGuardBranch()->getParent());
      return /*headerCFE &&*/ Gdominates && GpostDominates;
    }

    return headerCFE;
  }

  // Funzione helper per ottenere puntatori di solo load/store 
  Value *getPtrIfLoadOrStore(Instruction *I) {
    if (auto *LI = dyn_cast<LoadInst>(I))
      return LI->getPointerOperand();
    if (auto *SI = dyn_cast<StoreInst>(I))
      return SI->getPointerOperand();
    return nullptr;
  }

  // Funzione helper per controllare che i due valori stiano usando lo stesso oggetto
  bool sameUnderlyingObject(Value *P1, Value *P2) {
    if (!P1 || !P2) return false;
    Value *U1 = getUnderlyingObject(P1);
    Value *U2 = getUnderlyingObject(P2);
    return U1 == U2;
  }

  // Funzione helper per trasformare il contesto di SourceSCEV nel contesto di TargetLoop
  static const SCEV *translateSCEVToTargetLoop(const SCEV *SourceSCEV, Loop *TargetLoop, ScalarEvolution &SE) {
    
    // Controlliamo se l'equazione è un contatore di ciclo (AddRecExpr)
    if (auto *AddRec = dyn_cast<SCEVAddRecExpr>(SourceSCEV)) {
        
        // Creiamo la StoreRewritten: copiamo inizio e passo, ma cambiamo il padrone (TargetLoop)
        return SE.getAddRecExpr(
            AddRec->getStart(),
            AddRec->getStepRecurrence(SE),
            TargetLoop,
            AddRec->getNoWrapFlags()
        );
    }
    
    // Se non è un contatore (es. è un numero fisso come '5'), lo restituiamo così com'è
    return SourceSCEV;
}
  
  // CONDIZIONE 4
  // Non deve esistere un caso in cui L2 accede a un indirizzo "più avanti"
  // rispetto a quello prodotto/aggiornato da L1 alla stessa iterazione,
  // cioè una forma di "future read/write" -> negative distance.
  //
  // Implementazione:
  // - Considera solo load/store.
  // - Richiede stesso underlying object.
  // - Normalizza i due puntatori nello STESSO scope (L1), e confronta le differenze.
  // - Se non riesce a dimostrare che è safe -> blocca la fusione.
  bool hasNoNegativeDistanceDeps(Loop *L1, Loop *L2, ScalarEvolution &SE) {
    // 1. Raccogliamo SOLO le istruzioni di memoria di L1
    SmallVector<Instruction*, 32> MemInstsL1;
    for (BasicBlock *BB : L1->blocks()) {
      for (Instruction &I : *BB) {
        if (isa<LoadInst>(I) || isa<StoreInst>(I)) {
          MemInstsL1.push_back(&I);
        }
      }
    }

    // 2. Raccogliamo SOLO le istruzioni di memoria di L2
    SmallVector<Instruction*, 32> MemInstsL2;
    for (BasicBlock *BB : L2->blocks()) {
      for (Instruction &I : *BB) {
        if (isa<LoadInst>(I) || isa<StoreInst>(I)) {
          MemInstsL2.push_back(&I);
        }
      }
    }


    // Usiamo lo stesso scope per confrontare le due espressioni
    //Loop *Scope = L1;

    for (Instruction *I1 : MemInstsL1) {
      Value *P1 = getPtrIfLoadOrStore(I1);
      if (!P1) continue;

      for (Instruction *I2 : MemInstsL2) {
        // In caso di read-after-read non ho interesse nel fare controlli
        // dato che a prescindere non genera conflitti
        // A noi interessa gestire:
        // read-after-write, write-after-write e le write-after-read
        if (isa<LoadInst>(I1) && isa<LoadInst>(I2)) {
          continue;
        }

        Value *P2 = getPtrIfLoadOrStore(I2);
        if (!P2) continue;

        // Se sicuramente non è lo stesso oggetto, ignoriamo (nessuna dipendenza tra array diversi)
        if (!sameUnderlyingObject(P1, P2))
          continue;

        // Riscriviamo entrambi i puntatori nello stesso scope
        const SCEV *S1 = SE.getSCEVAtScope(P1, L1);
        const SCEV *S2 = SE.getSCEVAtScope(P2, L2);

        if (isa<SCEVCouldNotCompute>(S1) || isa<SCEVCouldNotCompute>(S2))
          return false;

        //Prendiamo S2 (che vive in L2) e lo traduciamo in L1
        const SCEV *S2_Rewritten = translateSCEVToTargetLoop(S2, L1, SE);
        // Differenza tra indirizzi (o offset) nel medesimo scope:
        // Delta = Ptr(L2) - Ptr(L1)
        const SCEV *Delta = SE.getMinusSCEV(S2_Rewritten, S1);

        // Controllo se Ptr(L2) è "più avanti" (delta > 0) rispetto a Ptr(L1)
        
        // - se SE può provare che Delta è > 0 => blocca
        if (SE.isKnownPositive(Delta)) {
          // L2 accede sistematicamente a indirizzi successivi rispetto a L1
          // es: load A[i+1] vs store A[i]
          return false;
        }

        // - se SE NON riesce a provare che Delta è <= 0 => blocca
        if (!SE.isKnownNonPositive(Delta)) {
          return false;
        }

        // Se Delta <= 0 provato, per questa coppia siamo ok:
        // L2 legge lo stesso elemento o uno "precedente"
      }
    }

    return true;
  }

  // Questa funzione controlla che due loop guarded abbiano la stessa semantica
  bool haveSameGuardSemantics(Loop *L1, Loop *L2) {
    BranchInst *Guard1 = L1->getLoopGuardBranch();
    BranchInst *Guard2 = L2->getLoopGuardBranch();

    if (!Guard1 || !Guard2 || !Guard1->isConditional() || !Guard2->isConditional())
      return false;

    auto *Cmp1 = dyn_cast<CmpInst>(Guard1->getCondition());
    auto *Cmp2 = dyn_cast<CmpInst>(Guard2->getCondition());

    // Se non sono istruzioni di comparazione, fallisce
    if (!Cmp1 || !Cmp2) 
      return false;

    // isIdenticalTo verifica che operandi e predicato siano esattamente gli stessi
    return Cmp1->isIdenticalTo(Cmp2);
  }

  void findFusionCandidates(const std::vector<Loop*> &Siblings, SmallVectorImpl<LoopPair> &Candidates, ScalarEvolution &SE, DominatorTree &DT, PostDominatorTree &PDT) {
    /*
    for (size_t i = 0; i < Siblings.size(); ++i) {
    Loop *L1 = Siblings[i];

    for (size_t j = i + 1; j < Siblings.size(); ++j) {
        Loop *L2 = Siblings[j];
        //logica per fondere L1 e L2
        //logica per fondere L2 e L1
    }
    }
    */
    for (Loop *L1 : Siblings) {
      bool isL1Guarded = (L1->getLoopGuardBranch() != nullptr);

      for (Loop *L2 : Siblings) {
        if (L1 == L2) continue;

        bool isL2Guarded = (L2->getLoopGuardBranch() != nullptr);
        outs() << "L1 guarded: " << isL1Guarded << " L2 guarded: " << isL2Guarded << "\n";
        if (isL1Guarded != isL2Guarded){
          outs() << "ERRORE: Il loop " << L1 << " ed il loop " << L2 << " non hanno entrambi una guardia\n";
          continue;
        }

        //Se sono guarded, controlliamo anche che abbiano la stessa semantica
        outs() << "Inizio controllo della semantica della guardia di "<< L1 <<" e " << L2 << "\n";
        if (isL1Guarded && !haveSameGuardSemantics(L1, L2)){
          outs() << "ERRORE: Il loop " << L1 << " ed il loop " << L2 << " non hanno la stessa guardia\n";
          continue;
        }
        outs() << "Fine controllo della semantica della guardia di "<< L1 <<" e " << L2 << ": SUPERATO\n";

        // Controllo l'adiacenza dei due loop
        outs() << "Inizio controllo adiacenza di "<< L1 <<" e " << L2 << "\n";
        if (!areLoopsAdjacent(L1, L2, isL1Guarded)){
          outs() << "ERRORE: Il loop " << L1 << " ed il loop " << L2 << " non sono adiacenti\n";
          continue;
        }
        outs() << "Fine controllo adiacenza di "<< L1 <<" e " << L2 << ": SUPERATO\n";

        // Controllo il numero di iterazioni dei due loop
        outs() << "Inizio controllo trip count di "<< L1 <<" e " << L2 << "\n";
        if (!haveSameIterationNumber(L1, L2, SE)){
          outs() << "ERRORE: Il loop " << L1 << " ed il loop " << L2 << " non hanno lo stesso numero di iterazioni\n";
          continue;
        }
        outs() << "Fine controllo adiacenza di "<< L1 <<" e " << L2 << ": SUPERATO\n";

        // Controllo che il Control Flow dei due loop sia equivalente
        outs() << "Inizio controllo di equivalenza di control flow di "<< L1 <<" e " << L2 << "\n";
        if (!isControlFlowEquivalent(L1, L2, DT, PDT)){
          outs() << "ERRORE: Il loop " << L1 << " ed il loop " << L2 << " non hanno un Control Flow equivalente\n";
          continue;
        }
        outs() << "Fine controllo di equivalenza di control flow di "<< L1 <<" e " << L2 << ": SUPERATO\n";

        // Controllo che i due loop non abbiano una NegativeDistanceDeps
        outs() << "Inizio controllo dipendenze di "<< L1 <<" e " << L2 << "\n";
        if (!hasNoNegativeDistanceDeps(L1, L2, SE)){
          outs() << "ERRORE: Il loop " << L1 << " ed il loop " << L2 << " hanno una NegativeDistanceDeps\n";
          continue;
        }
        outs() << "Fine controllo dipendenze di "<< L1 <<" e " << L2 << ": SUPERATO\n";

        Candidates.push_back({L1, L2, isL1Guarded});
      }
    }

    // Ricorsione nei sotto-livelli
    for (Loop *L : Siblings) {
      if (!L->getSubLoops().empty()) {
        findFusionCandidates(L->getSubLoopsVector(), Candidates, SE, DT, PDT);
      }
    }
  }

  // Tramite questa funzione cerco l'istruzione phi dentro il loop passato come parametro
  //E nello specifico la induction variable canonica
  static PHINode *findCanonicalIndVar(Loop *L) {
    BasicBlock *Header = L->getHeader();
    BasicBlock *Preheader = L->getLoopPreheader();
    BasicBlock *Latch = L->getLoopLatch();

    // controllo che esistano (sempre meglio essere sicuri)
    if (!Header || !Preheader || !Latch) return nullptr;

    for (PHINode &PN : Header->phis()) {
      Value *Init = PN.getIncomingValueForBlock(Preheader);
      Value *StepV = PN.getIncomingValueForBlock(Latch);
      if (!Init || !StepV) continue;

      // StepV deve essere un'istruzione che usa PN (es add PN, C)
      auto *StepI = dyn_cast<Instruction>(StepV);
      if (!StepI) continue;

      //bool UsesPN = false;

      for(User* u : PN.users()){
        if(u == StepI){
          //UsesPN = true;
          //break;
          return &PN;
        }
      }
      //return &PN;
    }
    return nullptr;
  }

  BasicBlock *getBodyEntry(Loop *L) {
    BranchInst *BI = dyn_cast<BranchInst>(L->getHeader()->getTerminator());

    //in tal caso il body è l'header stesso
    if (BI->isUnconditional()) {
      return L->getHeader();
    }
    
    if (L->contains(BI->getSuccessor(0)))
      return BI->getSuccessor(0);
    return BI->getSuccessor(1); 
  }

  Value *getPHIValue(BasicBlock *BB, BasicBlock *Pred) {
    for (PHINode &PHI : BB->phis()) {
      int id = PHI.getBasicBlockIndex(Pred);
        if (id != -1)
          return PHI.getIncomingValue(id);
    }
    return nullptr;
  }

  void setPHIValue(BasicBlock *BB, BasicBlock *Pred, Value* v) {
    for (PHINode &PHI : BB->phis()) {
      int id = PHI.getBasicBlockIndex(Pred);
        if (id != -1)
          PHI.setIncomingValue(id, v);
    }
  }

  struct LoopFusion : PassInfoMixin<LoopFusion> {
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
      LoopInfo &LI = AM.getResult<LoopAnalysis>(F);
      ScalarEvolution &SE = AM.getResult<ScalarEvolutionAnalysis>(F);
      DominatorTree &DT = AM.getResult<DominatorTreeAnalysis>(F);
      PostDominatorTree &PDT = AM.getResult<PostDominatorTreeAnalysis>(F);
      bool changed = false;

      // Rendo tutti i loop in forma canonica se non lo sono già
      for(Loop* l : LI)
        if(!l->isLoopSimplifyForm())
          simplifyLoop(l, &DT, &LI, &SE, nullptr, nullptr, false);
      
      SmallVector<LoopPair, 4> Candidates;
      
      std::vector<Loop*> TopLevelLoops(LI.begin(), LI.end());
      outs() << "Inizio ricerca candidati alla fusione\n";
      findFusionCandidates(TopLevelLoops, Candidates, SE, DT, PDT);
      outs() << "Fine ricerca candidati alla fusione\n";

      if (Candidates.size() <= 0){
        outs() << "Nessun candidato trovato per la fusione\n";
      }
      else{
        outs() << "Candidati trovati: inizio fusione\n";
      }

      // Una volta trovate le coppie di loop da fondere si passa alla fusion
      // (quindi trasformazione del codice della funzione)
      for (const LoopPair &P : Candidates) {
        Loop *L1 = P.L1;
        Loop *L2 = P.L2;
        
        // GetHeader under the hood accede al primo blocco del loop qualunque esso sia, 
        // di conseguenza se il loop è rotated restituirà il primo blocco del body
        BasicBlock *Header1  = L1->getHeader();
        BasicBlock *Header2  = L2->getHeader();

        // Primi basic blocks dei body dei rispettivi loop. Potrebbero coincidere con i latch
        BasicBlock *BodyEntry1 = getBodyEntry(L1);
        BasicBlock *BodyEntry2 = getBodyEntry(L2);

        BasicBlock *Latch1   = L1->getLoopLatch();
        BasicBlock *Latch2   = L2->getLoopLatch();
        BasicBlock *Exit1    = L1->getUniqueExitBlock();
        BasicBlock *Exit2    = L2->getUniqueExitBlock();
        
        if (!Header1 || !Header2 || !Latch1 || !Latch2 || !Exit1 || !Exit2) {
          outs() << "ERRORE nel loop della funzione: " << F.getName() << "\n"; 
          continue;
        }

        // Unifica IV
        PHINode *IV1 = findCanonicalIndVar(L1);
        PHINode *IV2 = findCanonicalIndVar(L2);
        if (!IV1 || !IV2) continue;

        IV2->replaceAllUsesWith(IV1);
        IV2->eraseFromParent();

        // Spostiamo le altre variabili PHI (es. accumulatori) da Header2 a Header1
        for (PHINode &PN : make_early_inc_range(Header2->phis())) {
          PN.moveBefore(Header1->getFirstNonPHI());
          int Preheader2Idx = PN.getBasicBlockIndex(L2->getLoopPreheader());
          if (Preheader2Idx != -1) {
            PN.setIncomingBlock(Preheader2Idx, L1->getLoopPreheader());
          }
        }

        // Header2 non punta più ad Exit2
        Exit2->removePredecessor(Header2);

        // La branch che, in caso di condizione del loop falsa, portava al blocco Exit1 ora porta ad Exit2
        BasicBlock *ExitingBlock1 = L1->getExitingBlock();

        if (ExitingBlock1) {
          ExitingBlock1->getTerminator()->replaceSuccessorWith(Exit1, Exit2);
        }
        
        // Conservo il valore che assume il phi node quando il flusso arriva da Latch1
        Value* phiValue = getPHIValue(Header1, Latch1);

        // Il Latch1 prima riportava all'Header1, ora porta al primo blocco del body di L2 per eseguire le istruzioni di quest'ultimo
        Latch1->getTerminator()->replaceSuccessorWith(Header1, BodyEntry2);

        // BodyEntry2 ora aspetta il flusso da Latch1, non più da Header2
        BodyEntry2->replacePhiUsesWith(Header2, Latch1);

        // Latch2 punta ora a Header1 invece che a Header2, diventando così il nuovo latch di L1
        Latch2->getTerminator()->replaceSuccessorWith(Header2, Header1);

        // Header1 aspetta il flusso da Latch2, non più da Latch1
        Header1->replacePhiUsesWith(Latch1, Latch2);
        setPHIValue(Header1, Latch2, phiValue);
      
        outs() << "Fine fusione\n";
        changed = true;
      }

      if (changed) {
        outs() << "La funzione " << F.getName() << " è stata modificata.\n";
        return PreservedAnalyses::none();
      }
      return PreservedAnalyses::all();
    }

    static bool isRequired() { return true; }
  };

} // Fine namespace anonimo

//-----------------------------------------------------------------------------

llvm::PassPluginLibraryInfo getFourthAssignmentPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "FourthAssignment", LLVM_VERSION_STRING,
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, FunctionPassManager &FPM,
          ArrayRef<PassBuilder::PipelineElement>) {
              if (Name == "lf") {
                FPM.addPass(LoopFusion());
                return true;
              }
              return false;
          });
    }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getFourthAssignmentPluginInfo();
}

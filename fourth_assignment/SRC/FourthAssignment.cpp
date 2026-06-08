#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/ScalarEvolutionExpressions.h"
#include "llvm/Analysis/ValueTracking.h" // GetUnderlyingObject

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

    // Controllo se L1 ha un solo punto di uscita.
    if (!ExitL1) return false;

    BasicBlock *PreheaderLoop2 = L2->getLoopPreheader();

    // Controllo il preheader di L2, verifico che esista e che sia "vuoto"
    if (!PreheaderLoop2 || !isBlockSafeForFusion(PreheaderLoop2)) return false;

    // Controllo se sono dei loop guarded
    if (areGuarded) {
      BranchInst *GuardBranchL2 = L2->getLoopGuardBranch();
      if (!GuardBranchL2) return false; 
      
      BasicBlock *GuardedBlockLoop2 = GuardBranchL2->getParent();

      // Controllo che anche la guardia di L2 sia vuota
      if (!isBlockSafeForFusion(GuardedBlockLoop2)) return false;

      // Verifico che l'uscita di L1 sbatta contro la guardia di L2
      return (ExitL1 == GuardedBlockLoop2);
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

  // Determina se gli step di incremento del contatore sono uguali
  bool sameSteps(Loop* L1, Loop* L2, ScalarEvolution &SE){
    PHINode *iv1 = L1->getInductionVariable(SE);
    PHINode *iv2 = L2->getInductionVariable(SE);
    SCEVConstant step1;
    SCEVConstant step2;
    if (auto *ar1 = dyn_cast<SCEVAddRecExpr>(SE.getSCEV(iv1)) && auto *ar2 = dyn_cast<SCEVAddRecExpr>(SE.getSCEV(iv2))){
        step1 = ar1->getStepRecurrence(SE);
        step2 = ar2->getStepRecurrence(SE);
    }
    // Controllo che i valori di step1 e step2 non siano indeterminati
    if((!SE.isKnownPositive(step1) && !SE.isKnownNegative(step1)) || (!SE.isKnownPositive(step2) && !SE.isKnownNegative(step2)))
        return false;
        
    return SE.getMinusDiff(step1, step2)->isZero();
  }

  // CONDIZIONE 3
  // Verifica Control Flow Equivalence
  bool isControlFlowEquivalent(Loop *L1, Loop *L2, DominatorTree &DT, PostDominatorTree &PDT) {
    // L1 deve dominare L2
    bool dominates = DT.dominates(L1->getHeader(), L2->getHeader());
    
    // L2 deve POST-dominare L1
    bool postDominates = PDT.dominates(L2->getHeader(), L1->getHeader());
    
    return dominates && postDominates;
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
    Value *U1 = GetUnderlyingObject(P1);
    Value *U2 = GetUnderlyingObject(P2);
    return U1 == U2;
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
    Loop *Scope = L1;

    for (Instruction *I1 : MemInstsL1) {
      Value *P1 = getPtrIfLoadOrStore(I1);
      if (!P1) continue;

      for (Instruction *I2 : MemInstsL2) {
        Value *P2 = getPtrIfLoadOrStore(I2);
        if (!P2) continue;

        // Se sicuramente non è lo stesso oggetto, ignoriamo (nessuna dipendenza tra array diversi)
        if (!sameUnderlyingObject(P1, P2))
          continue;

        // Riscriviamo entrambi i puntatori nello stesso scope
        const SCEV *S1 = SE.getSCEVAtScope(P1, Scope);
        const SCEV *S2 = SE.getSCEVAtScope(P2, Scope);

        if (isa<SCEVCouldNotCompute>(S1) || isa<SCEVCouldNotCompute>(S2))
          return false;

        // Differenza tra indirizzi (o offset) nel medesimo scope:
        // Delta = Ptr(L2) - Ptr(L1)
        const SCEV *Delta = SE.getMinusSCEV(S2, S1);

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
    for (Loop *L1 : Siblings) {
      bool isL1Guarded = (L1->getLoopGuardBranch() != nullptr);

      for (Loop *L2 : Siblings) {
        if (L1 == L2) continue;

        bool isL2Guarded = (L2->getLoopGuardBranch() != nullptr);

        if (isL1Guarded != isL2Guarded){
          outs() << "ERRORE: Il loop " << L1 << " ed il loop " << L2 << " non hanno entrambi una guardia";
          continue;
        }

        // Se sono guarded, controlliamo anche che abbiano la stessa semantica
        if (isL1Guarded && !haveSameGuardSemantics(L1, L2)){
          outs() << "ERRORE: Il loop " << L1 << " ed il loop " << L2 << " non hanno la stessa guardia";
          continue;
        }

        // Controllo: adiacenza, numero di iterazioni, dominanza/postdominanza e dipendenze
        if (areLoopsAdjacent(L1, L2, isL1Guarded) && 
            haveSameIterationNumber(L1, L2, SE) && 
            sameSteps(L1, L2, SE) && 
            isControlFlowEquivalent(L1, L2, DT, PDT) && 
            hasNoNegativeDistanceDeps(L1, L2, SE)) {
          Candidates.push_back({L1, L2, isL1Guarded});
        }
        
      }
    }

    // Ricorsione nei sotto-livelli
    for (Loop *L : Siblings) {
      if (!L->getSubLoops().empty()) {
        findFusionCandidates(L->getSubLoopsVector(), Candidates, SE, DT, PDT);
      }
    }
  }

  // Tramite questa fuzione cerco l'istruzione phi dentro il loop passato come parametro
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

      bool UsesPN = false;
      for (Value *Op : StepI->operands())
        if (Op == &PN) { UsesPN = true; break; }

      if (!UsesPN) continue;

      return &PN;
    }
    return nullptr;
  }

  struct LoopFusion : PassInfoMixin<LoopFusion> {
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
      LoopInfo &LI = AM.getResult<LoopAnalysis>(F);
      ScalarEvolution &SE = AM.getResult<ScalarEvolutionAnalysis>(F);
      DominatorTree &DT = AM.getResult<DominatorTreeAnalysis>(F);
      PostDominatorTree &PDT = AM.getResult<PostDominatorTreeAnalysis>(F);

      // Usato il nome corretto della Struct: LoopPair
      SmallVector<LoopPair, 4> Candidates;
      bool changed = false;

      std::vector<Loop*> TopLevelLoops(LI.begin(), LI.end());
      findFusionCandidates(TopLevelLoops, Candidates, SE, DT, PDT);

      // Una volta trovate le coppie di loop da fondere si passa alla fusion
      // (quindi trasformazione del codice della funzione)
      for (const LoopPair &P : Candidates) {
        Loop *L1 = P.L1;
        Loop *L2 = P.L2;

        BasicBlock *Header1 = L1->getHeader();
        BasicBlock *Header2 = L2->getHeader();
        BasicBlock *Latch1  = L1->getLoopLatch();
        BasicBlock *Latch2  = L2->getLoopLatch();
        BasicBlock *Exit2   = L2->getUniqueExitBlock();

        // controllo che esistano (sempre meglio essere sicuri) 
        if (!Header1 || !Header2 || !Latch1 || !Latch2 || !Exit2){
          outs() << "ERRORE: E' avvenuto un errore sconosciuto durante la lettura delle componenti dei loop " << L1 << " ed " << L2;
          continue;
        }
          

        // 1) Unifica IV
        PHINode *IV1 = findCanonicalIndVar(L1);
        PHINode *IV2 = findCanonicalIndVar(L2);
        if (!IV1 || !IV2){
          outs() << "ERRORE: Impossibile recuperare i PHI node dai loop " << L1 << " ed " << L2;
          continue;
        }
          

        IV2->replaceAllUsesWith(IV1);

        // 2) Rewire latch1: al posto di tornare a header1, vai a header2
        auto *BI1 = dyn_cast<BranchInst>(Latch1->getTerminator());
        if (!BI1 || !BI1->isConditional()) continue;

        // bisogna capire quale successor è "backedge" verso Header1:
        for (unsigned s = 0; s < BI1->getNumSuccessors(); ++s) {
          if (BI1->getSuccessor(s) == Header1) {
            BI1->setSuccessor(s, Header2);
          }
        }

        // 3) Rewire latch2: al posto di tornare a header2, torna a header1 (backedge unico)
        auto *BI2 = dyn_cast<BranchInst>(Latch2->getTerminator());
        if (!BI2 || !BI2->isConditional()) continue;

        for (unsigned s = 0; s < BI2->getNumSuccessors(); ++s) {
          if (BI2->getSuccessor(s) == Header2) {
            BI2->setSuccessor(s, Header1);
          }
        }

        // 4) Aggiustare PHI nodes in Header2 / Header1 se necessario
        // (Spesso necessario: quando cambi predecessori di un header devi aggiornare i PHI)
        // Qui serve codice extra per rimuovere incoming non più validi e aggiungerne di nuovi.

        changed = true;
      }

      if (changed) {
        outs() << "La funzione " << F.getName() << " è stata modificata.\n";
        PreservedAnalyses PA;
        PA.preserveSet<CFGAnalyses>();
        return PA;
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
              if (Name == "loop-fusion") {
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

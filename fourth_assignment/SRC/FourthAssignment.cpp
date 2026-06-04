#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/DependenceAnalysis.h"

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

  // Funzione usata per controllare se i blocchi di guardia/preheader dei due loop sono vuoti
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
  bool haveSameIterationNumber(Loop *L1, Loop *L2, ScalarEvolution &SE) {
    unsigned int TC1 = SE.getSmallConstantTripCount(L1);
    unsigned int TC2 = SE.getSmallConstantTripCount(L2);

    // Se ScalarEvolution non riesce a determinare un trip count costante “piccolo”, niente fusion.
    if (TC1 == 0 || TC2 == 0) {
      return false;
    }

    return TC1 == TC2;
  }

//Proposta modifica della condizione 2, questa variante gestisce anche se le condizioni di terminazione sono contenute dentro a variabili
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
    //https://discourse.llvm.org/t/how-to-compare-scevs/76174
    //spiegazione del perché posso fare comparazione dei 2 puntatori in modo sicuro
    return TC1 == TC2;
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


  // CONDIZIONE 4
  // Analisi delle dipendenze
  static bool hasNoDependence(Loop *L1, Loop *L2, DependenceInfo &DI) {
    // 1. Raccogliamo SOLO le istruzioni di memoria di L1
    //dato che forma SSA garantisce che una definizione venga sovrascritta
    SmallVector<Instruction *, 16> MemInstsL1;
    for (BasicBlock *BB : L1->blocks()) {
        for (Instruction &I : *BB) {
            // mayReadOrWriteMemory() filtra in automatico Load, Store e Call
            if (I.mayReadOrWriteMemory()) {
                MemInstsL1.push_back(&I);
            }
        }
    }

    // 2. Raccogliamo SOLO le istruzioni di memoria di L2
    SmallVector<Instruction *, 16> MemInstsL2;
    for (BasicBlock *BB : L2->blocks()) {
        for (Instruction &I : *BB) {
            if (I.mayReadOrWriteMemory()) {
                MemInstsL2.push_back(&I);
            }
        }
    }

    // 3. Prodotto Cartesiano: controlliamo ogni memoria di L1 contro ogni memoria di L2
    for (Instruction *I1 : MemInstsL1) {
        for (Instruction *I2 : MemInstsL2) {
            
            // Chiamiamo l'API della slide del prof
            // (il terzo parametro 'true' serve per cercare anche dipendenze loop-carried)
            if (auto Dep = DI.depends(I1, I2, true)) {
                
                // Se c'è una dipendenza, dobbiamo capire di che tipo è.
                // Se ENTRAMBE le istruzioni stanno solo leggendo (Input Dependence), 
                // non c'è conflitto e possiamo ignorarla.
                if (Dep->isInput()) {
                    continue; 
                }

                // Se arriviamo qui, abbiamo trovato una dipendenza RAW, WAR o WAW.
                // L'approccio più sicuro è dichiarare i loop NON fondibili.
                return false; 
            }
        }
    }
    return true;
  }

//Condizione 4 R2
 // Analisi delle dipendenze
  static bool hasNoDependence(Loop *L1, Loop *L2, DependenceInfo &DI) {
    // 1. Raccogliamo SOLO le istruzioni di memoria di L1
    //dato che forma SSA garantisce che una definizione venga sovrascritta
    SmallVector<Instruction *, 16> MemInstsL1;
    for (BasicBlock *BB : L1->blocks()) {
        for (Instruction &I : *BB) {
            // mayReadOrWriteMemory() filtra in automatico Load, Store e Call
            if (I.mayReadOrWriteMemory()) {
                MemInstsL1.push_back(&I);
            }
        }
    }

    // 2. Raccogliamo SOLO le istruzioni di memoria di L2
    SmallVector<Instruction *, 16> MemInstsL2;
    for (BasicBlock *BB : L2->blocks()) {
        for (Instruction &I : *BB) {
            if (I.mayReadOrWriteMemory()) {
                MemInstsL2.push_back(&I);
            }
        }
    }

    // 3. Prodotto Cartesiano: controlliamo ogni memoria di L1 contro ogni memoria di L2
    for (Instruction *I1 : MemInstsL1) {
        for (Instruction *I2 : MemInstsL2) {
            Value *PtrL1 = getLoadStorePointerOperand(I1);
            Value *PtrL2 = getLoadStorePointerOperand(I2);
            
            // 2. Le convertiamo in equazioni SCEV
            const SCEV *ScevL1 = SE.getSCEVAtScope(PtrL1, L1);
            const SCEV *ScevL2 = SE.getSCEVAtScope(PtrL2, L2);
            
            // 3. LA REGOLA D'ORO: Indice(L1) - Indice(L2)
            const SCEV *DistanzaMagica = SE.getMinusSCEV(ScevL1, ScevL2);
            
            // 4. Controllo definitivo
            if (SE.isKnownNegative(DistanzaMagica)) {
                // Se è negativo, significa che:
                // - O L2 sta leggendo dal futuro (RAW fallito)
                // - O L2 sta distruggendo dati non ancora letti (WAR fallito)
                return false; // BLOCCO LA FUSIONE
            }
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

  void findFusionCandidates(const std::vector<Loop*> &Siblings, SmallVectorImpl<LoopPair> &Candidates, ScalarEvolution &SE, DominatorTree &DT, PostDominatorTree &PDT, DependenceInfo &DI) {
      for (Loop *L1 : Siblings) {
          bool isL1Guarded = (L1->getLoopGuardBranch() != nullptr);

          for (Loop *L2 : Siblings) {
              if (L1 == L2) continue;

              bool isL2Guarded = (L2->getLoopGuardBranch() != nullptr);

              if (isL1Guarded != isL2Guarded) continue;

              // Se sono guarded, controlliamo anche che abbiano la stessa semantica
              if (isL1Guarded && !haveSameGuardSemantics(L1, L2)) continue;

              // Controllo: adiacenza, numero di iterazioni, dominanza/postdominanza e dipendenze
              if (areLoopsAdjacent(L1, L2, isL1Guarded) && haveSameIterationNumber(L1, L2, SE) && isControlFlowEquivalent(L1, L2, DT, PDT) && hasNoDependence(L1, L2, DI)) {
                  Candidates.push_back({L1, L2, isL1Guarded});
              }
          }
      }

      // Ricorsione nei sotto-livelli
      for (Loop *L : Siblings) {
          if (!L->getSubLoops().empty()) {
              findFusionCandidates(L->getSubLoopsVector(), Candidates, SE, DT, PDT, DI);
          }
      }
  }

  struct LoopFusion : PassInfoMixin<LoopFusion> {
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
      LoopInfo &LI = AM.getResult<LoopAnalysis>(F);
      ScalarEvolution &SE = AM.getResult<ScalarEvolutionAnalysis>(F);
      DominatorTree &DT = AM.getResult<DominatorTreeAnalysis>(F);
      PostDominatorTree &PDT = AM.getResult<PostDominatorTreeAnalysis>(F);
      DependenceInfo &DI = AM.getResult<DependenceAnalysis>(F);

      // Usato il nome corretto della Struct: LoopPair
      SmallVector<LoopPair, 4> Candidates;
      bool changed = false;

      std::vector<Loop*> TopLevelLoops(LI.begin(), LI.end());
      findFusionCandidates(TopLevelLoops, Candidates, SE, DT, PDT, DI);

      // TODO: Implementare qui l'effettiva fusione logica dei loop candidati
      // Se viene eseguita una fusione, impostare changed = true;

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

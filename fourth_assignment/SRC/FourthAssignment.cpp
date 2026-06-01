#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/IR/Instructions.h"

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

  // Verifica Control Flow Equivalence
  bool isControlFlowEquivalent(Loop *L0, Loop *L1, DominatorTree &DT, PostDominatorTree &PDT) {
    // L0 deve dominare L1
    bool dominates = DT.dominates(L0->getHeader(), L1->getHeader());
    
    // L1 deve POST-dominare L0
    bool postDominates = PDT.dominates(L1->getHeader(), L0->getHeader());
    
    return dominates && postDominates;
  }

  // NUOVA FUNZIONE: Controlla che due loop guarded abbiano la stessa semantica
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

  void findFusionCandidates(const std::vector<Loop*> &Siblings, SmallVectorImpl<LoopPair> &Candidates, DominatorTree &DT, PostDominatorTree &PDT) {
      for (Loop *L1 : Siblings) {
          bool isL1Guarded = (L1->getLoopGuardBranch() != nullptr);

          for (Loop *L2 : Siblings) {
              if (L1 == L2) continue;

              bool isL2Guarded = (L2->getLoopGuardBranch() != nullptr);

              if (isL1Guarded != isL2Guarded) continue;

              // Se sono guarded, controlliamo anche che abbiano la stessa semantica
              if (isL1Guarded && !haveSameGuardSemantics(L1, L2)) continue;

              // Controllo adiacenza e dominanza/postdominanza
              if (areLoopsAdjacent(L1, L2, isL1Guarded) && isControlFlowEquivalent(L1, L2, DT, PDT)) {
                  Candidates.push_back({L1, L2, isL1Guarded});
              }
          }
      }

      // Ricorsione nei sotto-livelli
      for (Loop *L : Siblings) {
          if (!L->getSubLoops().empty()) {
              findFusionCandidates(L->getSubLoopsVector(), Candidates, DT, PDT);
          }
      }
  }

  struct LoopFusion : PassInfoMixin<LoopFusion> {
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
      LoopInfo &LI = AM.getResult<LoopAnalysis>(F);
      DominatorTree &DT = AM.getResult<DominatorTreeAnalysis>(F);
      PostDominatorTree &PDT = AM.getResult<PostDominatorTreeAnalysis>(F);

      // Usato il nome corretto della Struct: LoopPair
      SmallVector<LoopPair, 4> Candidates;
      bool changed = false;

      std::vector<Loop*> TopLevelLoops(LI.begin(), LI.end());
      findFusionCandidates(TopLevelLoops, Candidates, DT, PDT);

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

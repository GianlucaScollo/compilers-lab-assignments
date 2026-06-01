
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Analysis/PostDominators.h"


using namespace llvm;

//-----------------------------------------------------------------------------
// FourthAssignment implementation
//-----------------------------------------------------------------------------

// struttura dati per salvare i dati relativi ad due loop adiacenti
struct LoopPair {
  Loop* L1 = nullptr;
  Loop* L2 = nullptr;
  bool areGuarded = false;
}



namespace {

  // struttura dati per salvare i dati relativi ad due loop adiacenti
struct LoopPair {
  Loop* L1 = nullptr;
  Loop* L2 = nullptr;
  bool areGuarded = false;
}; // <-- CORREZIONE 1: Aggiunto punto e virgola!

namespace {

  // funzione usata per controllare se i blocchi di guardia / preheader dei due loop sono vuoti
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
  // controllo se i due loop sono adiacenti tra di loro
  // questa funzione si aspetta che L1 precede L2 (e non il viceversa)
  bool areLoopsAdjacent(Loop *L1, Loop *L2, bool areGuarded) {

    // CORREZIONE 3: Nome variabile coerente e uso di getUniqueExitBlock per sicurezza
    BasicBlock *ExitL1 = L1->getUniqueExitBlock();

    // controllo se L1 ha un solo punto di uscita. Se null restituisco subito false.
    if (!ExitL1) return false;

    BasicBlock *PreheaderLoop2 = L2->getLoopPreheader();

    // controllo il preheader di L2, verifico che esista e che sia "vuoto"
    if (!PreheaderLoop2 || !isBlockSafeForFusion(PreheaderLoop2)) return false;

    // controllo se sono dei loop guarded
    if (areGuarded) {
      //Controllo in primis che l'istruzione di branch esista allo scopo di evitare segmentation fault      
      BranchInst *GuardBranchL2 = L2->getLoopGuardBranch();
      if (!GuardBranchL2) return false; // Se dicevano che era guarded ma non ha la guardia, fallisce!
      
      // Chiamiamo getParent() per ricavare il blocco del guard
      BasicBlock *GuardedBlockLoop2 = GuardBranchL2->getParent();

      // controllo che anche la guardia di L2 sia vuota
      if (!isBlockSafeForFusion(GuardedBlockLoop2)) return false;

      // verifico che l'uscita di L1 sbatta contro la guardia di L2
      return (ExitL1 == GuardedBlockLoop2);
    }

    // Se non sono guarded, verifico che l'uscita di L1 sbatta contro il preheader di L2
    return (ExitL1 == PreheaderLoop2);
  }

// Verifica Control Flow Equivalence
static bool isControlFlowEquivalent(Loop *L0, Loop *L1, DominatorTree &DT, PostDominatorTree &PDT) {
    // L0 deve dominare L1
    bool dominates = DT.dominates(L0->getHeader(), L1->getHeader());
    
    // L1 deve POST-dominare L0 (Attenzione all'ordine degli argomenti!)
    bool postDominates = PDT.dominates(L1->getHeader(), L0->getHeader());
    
    return dominates && postDominates;
}




  // fare controllo per capire nel caso in cui i loop sono guarded capire se hanno la stessa semantica
  



  struct LoopFusion: PassInfoMixin<LoopFusion> {
    // Main entry point, takes IR unit to run the pass on (&F) and the
    // corresponding pass manager (to be queried if need be)
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
      LoopInfo &LI = AM.getResult<LoopAnalysis>(F);
      DominatorTree &DT = AM.getResult<DominatorTreeAnalysis>(F);
      PostDominatorTree &PDT = AM.getResult<PostDominatorTreeAnalysis>(F);
  
      bool changed = false;

      for (auto *LoopIter : LI) {
        // ...
      }

      if (changed) {
        outs() << "La funzione " << F.getName() << " è stata modificata.\n";
        PreservedAnalyses PA;
        PA.preserveSet<CFGAnalyses>();
        return PA;
      }
      return PreservedAnalyses::all();
    }

    // Without isRequired returning true, this pass will be skipped for functions
    // decorated with the optnone LLVM attribute. Note that clang -O0 decorates
    // all functions with optnone.
    static bool isRequired() { return true; }
  };

} // namespace



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

// This is the core interface for pass plugins. It guarantees that 'opt' will
// be able to recognize STRUCTNAME when added to the pass pipeline on the
// command line, i.e. via '-passes=FLAGNAME'
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getFourthAssignmentPluginInfo();
}

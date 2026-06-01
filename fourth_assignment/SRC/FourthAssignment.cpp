
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

  // CONDIZIONE 1
  // controllo se i due loop sono adiacentri tra di loro
  // questa funzione si aspetta che L1 precede L2 (e non il viceversa)
  bool areLoopsAdjacent(Loop *L1, Loop *L2, bool areGuarded) {

    BasicBlock *EntryBlockLoop2 = L1->getExitBlock();

    // controllo se L1 ha un solo punto di uscita (ha dei break?)
    // quindi se EntryBlockLoop2 = null restituisco subito false 
    // (getExitBlock() restituisce null se esistono multiple uscite)
    if (!EntryBlockLoop2) return false;

    BasicBlock *PreheaderLoop2 = L2->getLoopPreheader();

    // controllo il preheder di L2, verifico che abbia un preheder (cioè che sia in 
    // forma normale) e che esso sia vuoto (cioè che abbia solo il terminatore)
    if (!PreheaderLoop2 || !(isBlockSafeForFusion(PreheaderLoop2))) return false;

    // controllo se sono dei loop guarded
    if (areGuarded) {
      
      // otteniamo il blocco di guardia L2
      BasicBlock *GuardedBlockLoop2 = L2->getLoopGuardBranch()->getParent();

      // controllo che anche la guardia di L2 sia vuota
      if (!(isBlockSafeForFusion(GuardedBlockLoop2))) return false;

      // verifico che siano uguali
      return (EntryBlockLoop2 == GuardedBlockLoop2);
    }

    // verifico che siano uguali
    return (EntryBlockLoop2 == PreheaderLoop2);
  }
  
  // funzione usata per controllare se i blocchi di guardia / preheder dei due loop sono vuoti
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

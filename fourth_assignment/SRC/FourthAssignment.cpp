
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Analysis/PostDominators.h"


using namespace llvm;

//-----------------------------------------------------------------------------
// FourthAssignment implementation
//-----------------------------------------------------------------------------

namespace {

    struct LoopFusion: PassInfoMixin<LoopFusion> {
        // Main entry point, takes IR unit to run the pass on (&F) and the
        // corresponding pass manager (to be queried if need be)
        PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
            LoopInfo &LI = AM.getResult<LoopAnalysis>(F);
            DominatorTree &DT = AM.getResult<DominatorTreeAnalysis>(F);
            PostDominatorTree &PDT = AM.getResult<PostDominatorTreeAnalysis>(F);
        
            bool changed = false;

            for (auto *LoopIter : LI) {
                Loop *L1 = *LoopIter; 

                BasicBlock *successor = L1->getExitBlock();

                // controllo se il nostro loop ha un solo punto di uscita (ha dei break?)
                if (!successor) continue;

                // controllo che il BB successore sia vuoto
                if (!successor->getFirstNonPHIOrDbg()->isTerminator()) continue;

                BasicBlock *header = successor->getSingleSuccessor();
                
                // controllo che header esista
                if (!header) continue;

                Loop *L2 = LI.getLoopFor(header);

                // controllo che il L2 esista, cioè che header sia un header di un loop
                if (!L2) continue;
                
                // controllo se i due loop non sono entrambi guarded oppure unguarded
                if (L1.isGuarded() != L2.isGuarded()) continue;
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

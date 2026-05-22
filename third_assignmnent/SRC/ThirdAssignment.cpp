
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Support/raw_ostream.h"   // Per outs()


using namespace llvm;

//-----------------------------------------------------------------------------
// ThirdAssignment implementation
//-----------------------------------------------------------------------------

namespace {

// LICM implementation
struct LoopInvariantCodeMotion: PassInfoMixin<LoopInvariantCodeMotion> {
  // Main entry point, takes IR unit to run the pass on (&F) and the
  // corresponding pass manager (to be queried if need be)
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {

  }

  // Without isRequired returning true, this pass will be skipped for functions
  // decorated with the optnone LLVM attribute. Note that clang -O0 decorates
  // all functions with optnone.
  static bool isRequired() { return true; }
};

} // namespace



//-----------------------------------------------------------------------------

llvm::PassPluginLibraryInfo getThirdAssignmentPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "ThirdAssignment", LLVM_VERSION_STRING,
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, FunctionPassManager &FPM,
          ArrayRef<PassBuilder::PipelineElement>) {
              if (Name == "loopICM") {
                FPM.addPass(LoopInvariantCodeMotion());
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
  return getThirdAssignmentPluginInfo();
}


#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

//-----------------------------------------------------------------------------
// FirstAssignment implementation
//-----------------------------------------------------------------------------

namespace {

// AlgebraicIdentity implementation
struct AlgebraicIdentity: PassInfoMixin<AlgebraicIdentity> {
  // Main entry point, takes IR unit to run the pass on (&F) and the
  // corresponding pass manager (to be queried if need be)
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {

    // iterating the BBs of the function
    for (auto IterBB = F.begin(); IterBB != F.end(); ++IterBB) {

    	BasicBlock &B = *IterBB;

	// iterating the instructions of the BB
	for (auto IterINST = B.begin(); IterINST != B.end(); ++IterINST) {

		// now we have a single instruction of a BB
		Instruction &I = *IterINST;

		// then we can control if we can optimize it

		// outs() << "Value of Opcode: " << I.getOpcode() << " | "<< I.getOpcodeName() << " (of this instruction: " << I << ")\n";

		int flag = 0;
		Value *op;

		// check if the operation of the instruction is an add (13)
		if (I.getOpcode() == 13) {
			for (auto IterOp = I.op_begin(); IterOp != I.op_end(); ++IterOp) {
				Value *Operand = *IterOp;
				// check if it is a constant
				if (ConstantInt *C = dyn_cast<ConstantInt>(Operand)) {
					if (C->getValue() == 0) {
						flag = 1;
						op = Operand;
					}
				}
			}
		}
		// check if the operation of the instruction is an mul (17)
		if (I.getOpcode() == 17) {
        for (auto IterOp = I.op_begin(); IterOp != I.op_end(); ++IterOp) {
          Value *Operand = *IterOp;
          // check if it is a constant
          if (ConstantInt *C = dyn_cast<ConstantInt>(Operand)) {
            if (C->getValue() == 1) {
              flag = 1;
              op = Operand;
            }
          }
        }
      }

		if (flag) {
			// now i will replace all uses with the operand not null
			auto op1 = I.getOperand(0);
			auto op2 = I.getOperand(1);
			if (op == op1) {
				I.replaceAllUsesWith(op2);
			}
			else {
				I.replaceAllUsesWith(op1);
			}
		}
	}
    }

    return PreservedAnalyses::all();
  }

  // Without isRequired returning true, this pass will be skipped for functions
  // decorated with the optnone LLVM attribute. Note that clang -O0 decorates
  // all functions with optnone.
  static bool isRequired() { return true; }
};

// StrenghtReduction implementation
struct StrenghtReduction: PassInfoMixin<StrenghtReduction> {
  // Main entry point, takes IR unit to run the pass on (&F) and the
  // corresponding pass manager (to be queried if need be)
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {

    return PreservedAnalyses::all();
  }

  // Without isRequired returning true, this pass will be skipped for functions
  // decorated with the optnone LLVM attribute. Note that clang -O0 decorates
  // all functions with optnone.
  static bool isRequired() { return true; }
};

// MultiInstructionOptimization implementation
struct MultiInstructionOptimization: PassInfoMixin<MultiInstructionOptimization> {
  // Main entry point, takes IR unit to run the pass on (&F) and the
  // corresponding pass manager (to be queried if need be)
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {

    return PreservedAnalyses::all();
  }

  // Without isRequired returning true, this pass will be skipped for functions
  // decorated with the optnone LLVM attribute. Note that clang -O0 decorates
  // all functions with optnone.
  static bool isRequired() { return true; }
};

} // namespace



//-----------------------------------------------------------------------------

llvm::PassPluginLibraryInfo getFirstAssignmentPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "FirstAssignment", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
		  // adding pass by the value of flag for algebraic identity opt
                  if (Name == "alg-id") {
                    FPM.addPass(AlgebraicIdentity());
                    return true;
                  }
		  // adding pass by the value of flag for strenght reduction opt
		  if (Name == "str-rd") {
                    FPM.addPass(StrenghtReduction());
                    return true;
                  }
		  // adding pass by the value of flag for multi instruction opt
		  if (Name == "mul-ins-opt") {
                    FPM.addPass(MultiInstructionOptimization());
                    return true;
                  }
		  // flag for doing all the passes
		  if ((Name == "A") or (Name == "all")) {
		    FPM.addPass(AlgebraicIdentity());
	            FPM.addPass(StrenghtReduction());
		    FPM.addPass(MultiInstructionOptimization());
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
  return getFirstAssignmentPluginInfo();
}

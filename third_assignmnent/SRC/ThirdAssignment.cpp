
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Argument.h"


using namespace llvm;

//-----------------------------------------------------------------------------
// ThirdAssignment implementation
//-----------------------------------------------------------------------------

namespace {


  //Vincolo 1: È Loop Invariant?
  bool isLoopInvariant(Instruction &I, Loop *L) {
      for (Value *Operand : I.operands()) {
          //Use &U
          //Value *Operand = U.get();
          //verifica se è argomento di una funzione l'operando o se è una costante
          if (isa<Constant>(Operand) || isa<Argument>(Operand)) continue;
          
          if (Instruction *InstOp = dyn_cast<Instruction>(Operand)) {
              if (!L->contains(InstOp)) continue; // Definita fuori dal loop
              return false; // Definita dentro il loop, non è invariante
              //ipotizziamo che le istruzione vengano scansionate in ordine sequenziale
              //di conseguenza se un istruzione ha come operando un'altra istruzione che è definita dentro il loop sicuramente l'operando è associato un'altra istruzione che non è loop invariant

          }
      }
      return true; 
  }

  // 2. Vincolo 2: Il blocco domina tutte le uscite?
  bool dominatesAllExits(Instruction &I, Loop *L, DominatorTree &DT) {
      BasicBlock *InstBB = I.getParent();
      SmallVector<BasicBlock *, 4> ExitBlocks;
      L->getExitBlocks(ExitBlocks);

      for (BasicBlock *ExitBB : ExitBlocks) {
          if (!DT.dominates(InstBB, ExitBB)) {
              return false;
          }
      }
      return true;
  }

  // 3. La funzione principale che verifica se un'istruzione è candidata al LICM
  bool isCandidateForCodeMotion(Instruction &I, Loop *L, DominatorTree &DT) {
      // Escludiamo istruzioni di salto, chiamate a funzione, e operazioni di memoria 
      // (perché la memoria non rispetta le regole SSA )
      if (I.isTerminator() || I.mayHaveSideEffects() || I.mayReadOrWriteMemory()) {
          return false;
      }

      // Vincolo 1: Loop Invariant
      if (!isLoopInvariant(I, L)) {
          return false;
      }

      // Vincolo 2: Domina tutte le uscite
      if (!dominatesAllExits(I, L, DT)) {
          return false;
      }

      // Vincolo 3 e 4: Automaticamente superati in LLVM grazie alla forma SSA!
      // Vincolo 3: Usando SSA è escluso a livello di rappresentazione che più di un'istruzione possa ridefinire la stessa variabile.
      // Vincolo 4: è da escludere che una definizione non domini tutti i suoi usi, dato che in SSA ogni variabile è definita una sola volta e ogni uso è dominato dalla definizione. (c'è una singa reaching definition)
      // In linea teorica, anche il vincolo sulla dominanza delle uscite sarebbe rilassabile 
      // dato che non andrebbe in ogni caso a uccidere altre reaching definitions (portarlo fuori introdurrebbe del death code nel peggior caso) ma è comunque buona pratica applicarlo qualora fossero eventualmente presenti
      //istruzioni semanticamente sbagliate come divisioni per zero.
      
      return true;
  }
  // LICM implementation
  struct LoopInvariantCodeMotion: PassInfoMixin<LoopInvariantCodeMotion> {
    // Main entry point, takes IR unit to run the pass on (&F) and the
    // corresponding pass manager (to be queried if need be)
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
      // 1. Chiediamo a LLVM tutti i loop (Lui ce li dà in Pre-Order, dall'esterno all'interno)
      auto Loops = LI.getLoopsInPreorder();

      // 2. Li iteriamo AL CONTRARIO (Reverse Iterator)
      // In questo modo partiamo dai loop più profondi/innestati e risaliamo verso l'esterno.
      for (auto it = Loops.rbegin(); it != Loops.rend(); ++it) {
          Loop *L = *it; 
          
          BasicBlock *Preheader = L->getLoopPreheader();
          if (!Preheader) continue;
          
          Instruction *PreheaderTerminator = Preheader->getTerminator();
          bool changed_CFG = false;

          bool changed = true;
          while (changed) {
              changed = false;
              
              for (BasicBlock *BB : L->getBlocks()) {
                  for (Instruction &I : llvm::make_early_inc_range(*BB)) {
                      
                      if (isCandidateForCodeMotion(I, L, DT)) {
                          errs() << "Sposto l'istruzione: " << I << "\n";
                          I.moveBefore(PreheaderTerminator);
                          changed = true;
                          changed_CFG = true;
                      }
                  }
              }
          }
      }
      if (changed_CFG) {
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

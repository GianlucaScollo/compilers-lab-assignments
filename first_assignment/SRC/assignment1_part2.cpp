



















#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Constants.h"
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/bit.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define CONCAT_HELPER(pref, val, suf) pref ## val ## suf
#define CREATE_FUNC_NAME(pref, val, suf) CONCAT_HELPER(pref, val, suf)

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define STRUCTNAME FirstAssignment
#define FLAGNAME first-assignment


//-----------------------------------------------------------------------------
// FirstAssignment implementation
//-----------------------------------------------------------------------------

namespace {


  // 1. INSERISCI LA FUNZIONE QUI
  void computeNAF(uint64_t x, uint64_t &np, uint64_t &nm) {
      uint64_t xh = x >> 1;
      uint64_t x3 = x + xh;
      uint64_t c = xh ^ x3;
      
      np = x3 & c;
      nm = xh & c;
  }
/*
// New PM implementation
struct STRUCTNAME: PassInfoMixin<STRUCTNAME> {
  // Main entry point, takes IR unit to run the pass on (&F) and the
  // corresponding pass manager (to be queried if need be)
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {

    outs() << "nice\n";

    
    for (auto Iter = F.begin(); Iter != F.end(); ++Iter) {
      BasicBlock &B = *Iter;
       // Preleviamo le prime due istruzioni del BB
      Instruction &Inst1st = *B.begin(), &Inst2nd = *(++B.begin());

      // L'indirizzo della prima istruzione deve essere uguale a quello del 
      // primo operando della seconda istruzione (per costruzione dell'esempio)
      assert(&Inst1st == Inst2nd.getOperand(0));

      // Stampa la prima istruzione
      outs() << "PRIMA ISTRUZIONE: " << Inst1st << "\n";
      // Stampa la prima istruzione come operando
      outs() << "COME OPERANDO: ";
      Inst1st.printAsOperand(outs(), false);
      outs() << "\n";

      // User-->Use-->Value
      outs() << "I MIEI OPERANDI SONO:\n";
      for (auto Iter = Inst1st.op_begin(); Iter != Inst1st.op_end(); ++Iter) {
        Value *Operand = *Iter;

        if (Argument *Arg = dyn_cast<Argument>(Operand)) {
          outs() << "\t" << *Arg << ": SONO L'ARGOMENTO N. " << Arg->getArgNo() 
  	         <<" DELLA FUNZIONE " << Arg->getParent()->getName()
                 << "\n";
        }
        if (ConstantInt *C = dyn_cast<ConstantInt>(Operand)) {
          outs() << "\t" << *C << ": SONO UNA COSTANTE INTERA DI VALORE " << C->getValue()
                 << "\n";
        }
      }

      outs() << "LA LISTA DEI MIEI USERS:\n";
      for (auto Iter = Inst1st.user_begin(); Iter != Inst1st.user_end(); ++Iter) {
        //outs() << "\t" << *(dyn_cast<Instruction>(*Iter)) << "\n";
	User *U = *Iter;
        outs() << "\t" << *U << "\n";
      }

      // Qual è la differenza tra gli USERS e gli USES?
      // Prova a vedere cosa succede se in Foo.ll la seconda istruzione diventa
      // %4 = mul nsw i32 %3, %3
      outs() << "E DEI MIEI USI:\n";
      for (auto Iter = Inst1st.use_begin(); Iter != Inst1st.use_end(); ++Iter) {
        //outs() << "\t" << *(dyn_cast<Instruction>(Iter->getUser())) << "\n";
	Use &U = *Iter;
        outs() << "\t" << *U.getUser() << "@operand" << U.getOperandNo() << "\n";
      }

      // Manipolazione delle istruzioni
      Instruction *NewInst = BinaryOperator::Create(
          Instruction::Add, Inst1st.getOperand(0), Inst1st.getOperand(0));

      NewInst->insertAfter(&Inst1st);
      // Si possono aggiornare le singole references separatamente?
      // Controlla la documentazione e prova a rispondere.
      Inst1st.replaceAllUsesWith(NewInst);
    }
    
  // Without isRequired returning true, this pass will be skipped for functions
  // decorated with the optnone LLVM attribute. Note that clang -O0 decorates
  // all functions with optnone.
  static bool isRequired() { return true; }
};

    */

  struct STRUCTNAME: PassInfoMixin<STRUCTNAME> {
  // Main entry point, takes IR unit to run the pass on (&F) and the
  // corresponding pass manager (to be queried if need be)
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {

    //outs() << "nice\n";
    for (auto IterBB = F.begin(); IterBB != F.end(); ++IterBB) {
      BasicBlock &B = *IterBB;
      for (auto IterINSTR = B.begin(); IterINSTR != B.end();) {
        //salvo istruzione attuale e poi incremento iteratore per evitare problemi di invalidazione dopo eventuale cancellazione dell'istruzione
        Instruction &Inst = *IterINSTR++;
        if (auto *BinOp = dyn_cast<BinaryOperator>(&Inst)) {
            
            // STEP 2: Ok, è un'operazione binaria. Ma è una moltiplicazione?
            // Ogni istruzione in LLVM ha un "codice operativo" (Opcode) che la identifica.
            if (BinOp->getOpcode() == Instruction::Mul) {
              // NUOVA AGGIUNTA 1: Prendo i due pezzi della moltiplicazione
                Value *Op0 = BinOp->getOperand(0); // Quello a sinistra del *
                Value *Op1 = BinOp->getOperand(1); // Quello a destra del *
                
                // NUOVA AGGIUNTA 2: Controllo se sono dei numeri fissi (costanti)
                ConstantInt *CI0 = dyn_cast<ConstantInt>(Op0);
                ConstantInt *CI1 = dyn_cast<ConstantInt>(Op1);

                int MinCost = 999;
                Value *BestVariable = nullptr;
                uint64_t BestNP = 0;
                uint64_t BestNM = 0;
                bool WasNegative = false;

                //Numero 1
                if (CI1 != nullptr) {
                    const APInt &Val1 = CI1->getValue();
                    
                    // Lavoriamo solo con numeri fino a 64 bit per sicurezza
                    if (Val1.getActiveBits() <= 64) {
                        
                        // 1. Ci salviamo se era negativo
                        bool IsNeg = Val1.isNegative();
                        
                        // 2. Togliamo i bit di segno prendendo il valore assoluto pulito
                        uint64_t AbsVal = Val1.abs().getZExtValue();
                        
                        // 3. Calcoliamo le maschere NAF usando la tua funzione
                        uint64_t np = 0;
                        uint64_t nm = 0;
                        computeNAF(AbsVal, np, nm);
                        
                        // 4. Contiamo quanti '1' ci sono nelle maschere
                        int cost = llvm::popcount(np) + llvm::popcount(nm);
                        
                        // 5. Salviamo i risultati
                        if (cost < MinCost) {
                            MinCost = cost;
                            BestVariable = Op0; // Se Op1 è la costante, Op0 è la variabile da shiftare
                            BestNP = np;
                            BestNM = nm;
                            WasNegative = IsNeg;
                        }
                    }
                }

                if (CI0 != nullptr) {
                    const APInt &Val0 = CI0->getValue();
                    
                    if (Val0.getActiveBits() <= 64) {
                        
                        bool IsNeg = Val0.isNegative();
                        uint64_t AbsVal = Val0.abs().getZExtValue();
                        
                        uint64_t np = 0;
                        uint64_t nm = 0;
                        computeNAF(AbsVal, np, nm);
                        
                        int cost = llvm::popcount(np) + llvm::popcount(nm);
                        
                        // Se questa costante costa MENO di quella trovata prima, la sovrascrive
                        if (cost < MinCost) {
                            MinCost = cost;
                            BestVariable = Op1; // Se Op0 è la costante, Op1 è la variabile da shiftare
                            BestNP = np;
                            BestNM = nm;
                            WasNegative = IsNeg;
                        }
                    }
                }
                if (MinCost >= 1 && MinCost <= 3) {
                  if(WasNegative) {
                      std::swap(BestNP, BestNM);
                  }

                  IRBuilder<> Builder(&Inst);
                    
                  // Ci serve sapere se stiamo lavorando con i32, i64, ecc.
                  Type *Ty = Inst.getType();
                  
                  // Questa variabile terrà in memoria la nostra "catena" di calcoli
                  // man mano che aggiungiamo shift, somme e sottrazioni.
                  Value *Accumulatore = nullptr;

                  // ================================================================
                    // FASE 1: Generiamo la catena di Istruzioni di ADDIZIONE (NP)
                    // ================================================================
                    unsigned BitWidth = Ty->getIntegerBitWidth(); // Quanti bit ha la variabile (es. 32)
                    uint64_t mask_add = BestNP;
                    
                    while (mask_add != 0) {
                        // Trova la posizione del primo '1' partendo da destra (es: se è 8, restituisce 3)
                        unsigned shift_amt = llvm::countr_zero(mask_add);

                        if (shift_amt >= BitWidth) {
                            mask_add &= (mask_add - 1);
                            continue; 
                          }
                        
                        // Creiamo l'istruzione di Shift. 
                        // Se lo shift è 0, non creiamo un'istruzione inutile "x << 0", usiamo direttamente la variabile.
                        Value *Shl;
                        if (shift_amt == 0) {
                            Shl = BestVariable;
                        } else {
                            Value *ShiftConst = ConstantInt::get(Ty, shift_amt);
                            Shl = Builder.CreateShl(BestVariable, ShiftConst, "naf_shl");
                        }

                        // Agganciamo l'istruzione alla catena
                        if (Accumulatore == nullptr) {
                            // È la prima istruzione della catena
                            Accumulatore = Shl;
                        } else {
                            // Creiamo un'istruzione ADD che prende la catena precedente e somma il nuovo Shift
                            Accumulatore = Builder.CreateAdd(Accumulatore, Shl, "naf_add");
                        }
                        
                        // Trucco bitwise per spegnere l'ultimo '1' processato e passare al prossimo
                        mask_add &= (mask_add - 1);
                    }

                    // ================================================================
                    // FASE 2: Generiamo la catena di Istruzioni di SOTTRAZIONE (NM)
                    // ================================================================
                    uint64_t mask_sub = BestNM;
                    
                    while (mask_sub != 0) {
                        unsigned shift_amt = llvm::countr_zero(mask_sub);
                        
                        if (shift_amt >= BitWidth) {
                            mask_sub &= (mask_sub - 1);
                            continue; 
                        }
                        Value *Shl;
                        if (shift_amt == 0) {
                            Shl = BestVariable;
                        } else {
                            Value *ShiftConst = ConstantInt::get(Ty, shift_amt);
                            Shl = Builder.CreateShl(BestVariable, ShiftConst, "naf_shl");
                        }

                        if (Accumulatore == nullptr) {
                            // Caso teorico in cui la maschera delle addizioni era vuota.
                            // Creiamo un'istruzione NEG (nega) per fare "0 - shift"
                            Accumulatore = Builder.CreateNeg(Shl, "naf_neg");
                        } else {
                            // Creiamo un'istruzione SUB che sottrae il nuovo Shift dalla catena
                            Accumulatore = Builder.CreateSub(Accumulatore, Shl, "naf_sub");
                        }
                        
                        mask_sub &= (mask_sub - 1);
                    }

                    if (Accumulatore != nullptr) {
                        // Diciamo a LLVM: "Tutte le altre istruzioni che usavano il risultato
                        // della vecchia moltiplicazione, ora devono usare la fine della nostra catena"
                        Inst.replaceAllUsesWith(Accumulatore);
                        
                        // Ora la vecchia moltiplicazione è inutile e isolata. La eliminiamo fisicamente dal blocco.
                        Inst.eraseFromParent();
                        
                        // (Opzionale ma utile) Stampa un messaggio a terminale per farti vedere che ha funzionato
                        outs() << "Ottimizzazione NAF applicata con successo!\n";
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




} // namespace

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo CREATE_FUNC_NAME(get, STRUCTNAME, PluginInfo)() {
  return {LLVM_PLUGIN_API_VERSION, STR(STRUCTNAME), LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == STR(FLAGNAME)) {
                    FPM.addPass(STRUCTNAME());
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
  return CREATE_FUNC_NAME(get, STRUCTNAME, PluginInfo)();
}

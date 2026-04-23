
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h" // Per outs()
#include "llvm/Passes/PassBuilder.h"

// --- Librerie per la logica matematica e IR del tuo passo NAF ---
#include "llvm/IR/PassManager.h"      // Per PassInfoMixin e PreservedAnalyses
#include "llvm/IR/Function.h"         // Per Function
#include "llvm/IR/BasicBlock.h"       // Per BasicBlock
#include "llvm/IR/Instruction.h"      // Per Instruction
#include "llvm/IR/Instructions.h"     // Per BinaryOperator
#include "llvm/IR/Constants.h"        // Per ConstantInt
#include "llvm/IR/IRBuilder.h"        // Per IRBuilder (shift, add, sub)
#include "llvm/IR/Type.h"             // Per gestire i bit (i32, i64)
#include "llvm/Support/Casting.h"     // Per dyn_cast<>
#include "llvm/Support/MathExtras.h"  // Per popcount e countr_zero
#include "llvm/ADT/APInt.h"           // Per i numeri interi grandi
#include "llvm/Support/DivisionByConstantInfo.h"

// --- Librerie standard C++ ---
#include <algorithm>                  // Per std::swap
#include <cstdint>                    // Per uint64_t


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
      for (auto IterINST = B.begin(); IterINST != B.end(); ) {

        // now we have a single instruction of a BB
        Instruction &I = *IterINST;

        // then we can control if we can optimize it

        int flag = 0;
        Value *operandToKeep = nullptr;

        // check if the operation of the instruction is an add (13)
        if (I.getOpcode() == 13) {
          auto op1 = I.getOperand(0);
          auto op2 = I.getOperand(1);

          // check if op1 is 0
          if (ConstantInt *C = dyn_cast<ConstantInt>(op1)) {
            if (C->getValue() == 0) {
              flag = 1;
              operandToKeep = op2;
            }
          }

          // check if op2 is 0
          if (ConstantInt *C = dyn_cast<ConstantInt>(op2)) {
            if (C->getValue() == 0) {
              flag = 1;
              operandToKeep = op1;
            }
          }
        }

        // check if the operation of the instruction is an mul (17)
        if (I.getOpcode() == 17) {
          auto op1 = I.getOperand(0);
          auto op2 = I.getOperand(1);

          // check if op1 is 1
          if (ConstantInt *C = dyn_cast<ConstantInt>(op1)) {
            if (C->getValue() == 1) {
              flag = 1;
              operandToKeep = op2;
            }
          }

          // check if op2 is 1
          if (ConstantInt *C = dyn_cast<ConstantInt>(op2)) {
            if (C->getValue() == 1) {
              flag = 1;
              operandToKeep = op1;
            }
          }
        }

        if (flag && operandToKeep) {
          // now i will replace all uses of the instruction with the right operand
          I.replaceAllUsesWith(operandToKeep);
          // before the deletion of the instruction i will increment the iterator because i want a valid pointer for the next instruction
          ++IterINST;
          // now i can delete the instruction
          I.eraseFromParent();
        }
        else {
          ++IterINST;
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
void computeNAF(const APInt &x, APInt &np, APInt &nm) {

      APInt xh = x.lshr(1); // Shift logico a destra di 1 = x << 1
      APInt x3 = x + xh;
      APInt c = xh ^ x3;
      
      np = x3 & c;
      nm = xh & c;
  }


    struct StrengthReductionPass: PassInfoMixin<StrengthReductionPass> {
    // Main entry point, takes IR unit to run the pass on (&F) and the
    // corresponding pass manager (to be queried if need be)
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {

        //outs() << "nice\n";
        bool Changed = false;
        for (auto IterBB = F.begin(); IterBB != F.end(); ++IterBB) {
        BasicBlock &B = *IterBB;
        for (auto IterINSTR = B.begin(); IterINSTR != B.end();) {
            //salvo istruzione attuale e poi incremento iteratore per evitare problemi di invalidazione dopo eventuale cancellazione dell'istruzione
            Instruction &Inst = *IterINSTR++;
            if (auto *BinOp = dyn_cast<BinaryOperator>(&Inst)) {
                
                Type *Ty = Inst.getType();
                    
                    // SCUDO: Deve essere un intero (Niente Float, niente Vettori)
                    if (!Ty->isIntegerTy()) {
                        continue;
                    }

                    // =========================================================
                    // OTTIMIZZAZIONE DELLA MOLTIPLICAZIONE (NAF)
                    // =========================================================
                    if (BinOp->getOpcode() == Instruction::Mul) {
                        Value *Op0 = BinOp->getOperand(0); 
                        Value *Op1 = BinOp->getOperand(1); 
                        
                        ConstantInt *CI0 = dyn_cast<ConstantInt>(Op0);
                        ConstantInt *CI1 = dyn_cast<ConstantInt>(Op1);

                        int MinCost = 999; //upper bound alto
                        Value *BestVariable = nullptr;
                        
                        // Inizializziamo a vuoto, non serve assegnare 0
                        APInt BestNP; 
                        APInt BestNM; 
                        bool WasNegative = false;

                        // Analisi Operando 1, se vale nullptr significa che dynamic cast è fallito perché è una variabile
                        if (CI1 != nullptr) {
                            const APInt &Val1 = CI1->getValue();
                            bool IsNeg = Val1.isNegative();
                            APInt AbsVal = Val1.abs(); // Manteniamo la grandezza originale
                            
                            // Prepariamo np e nm con gli stessi bit di AbsVal e inizializzati a 0
                            unsigned bitWidth = Val1.getBitWidth();
                            APInt np(bitWidth, 0);
                            APInt nm(bitWidth, 0);
                            
                            computeNAF(AbsVal, np, nm);
                            
                            // APInt sa calcolare il suo Peso di Hamming in automatico! (calcoliamo numero di bit a 1, serve per sapere numero esatto di operazioni di shift che dovremo eseguire)
                            int cost = np.popcount() + nm.popcount(); 
                            
                            if (cost < MinCost) {
                                MinCost = cost;
                                BestVariable = Op0; 
                                BestNP = np;
                                BestNM = nm;
                                WasNegative = IsNeg;
                            }
                        }

                        // Analisi Operando 2 analoga al precedente operando
                        if (CI0 != nullptr) {
                            const APInt &Val0 = CI0->getValue();
                            bool IsNeg = Val0.isNegative();
                            APInt AbsVal = Val0.abs();
                            
                            unsigned bitWidth = Val0.getBitWidth();
                            APInt np(bitWidth, 0);
                            APInt nm(bitWidth, 0);
                            
                            computeNAF(AbsVal, np, nm);
                            
                            int cost = np.popcount() + nm.popcount(); 
                            //controllo quale dei 2 operandi abbia costo minore e salvo info di quello meno costose che richiede meno istruzioni per essere ottimizzato
                            if (cost < MinCost) {
                                MinCost = cost;
                                BestVariable = Op1; 
                                BestNP = np;
                                BestNM = nm;
                                WasNegative = IsNeg;
                            }
                        }
                        //Uso peso di hamming come metrica dato che nei compilatori moderni istruzioni come LEA nei compilatori x86 permette 
                        //di eseguire operazioni di shift e add in un'unica istruzione.
                        //Se si desidera metrica accurata si può usare formula 2*MinCost-1 + (Val1.isOdd() ? -1 : 0) che tiene conto anche delle operazioni di somma e sottrazione aggiuntive e a cui sottraiamo una shift se il numero è dispari e pertanto ha bit a 1 nella prima posizione (dove non abbiamo bisogno di fare shift)
                        if (MinCost >= 1 && MinCost <= 3) {
                            //se il numero è negativo invertiamo le maschere in modo tale che risultato ottenuto sia negato rispetto al caso positivo, in questo modo possiamo gestire anche i numeri negativi senza dover aggiungere ulteriori operazioni di negazione che aumenterebbero il costo totale
                            if(WasNegative) {
                                std::swap(BestNP, BestNM);
                            }
                            //Questo ci risparmia un istruzione aggiuntiva per negare l'intero risultato.

                            //creiamo il builder
                            IRBuilder<> Builder(&Inst);
                            Value *Accumulatore = nullptr;
                            unsigned BitWidth = Ty->getIntegerBitWidth(); 

                            // FASE 1: Addizioni (Ciclo while aggiornato per APInt)
                            APInt mask_add = BestNP;
                            //usciamo dal while quando maschera è completamente processata (tutti i bit a 1 sono stati gestiti e spenti)
                            while (!mask_add.isZero()) {
                                // contiamo numero di trailing zero per trovare posizione del bit a 1 più a destra, che corrisponde al numero di zeri presenti alla sua destra.
                                unsigned shift_amt = mask_add.countr_zero();
                                
                                //Programmazione difensiva: se shift_amt è maggiore o uguale alla larghezza in bit del tipo, significa che abbiamo un bit a 1 in una posizione non valida (oltre la dimensione del tipo), quindi ignoriamo quel bit e passiamo al successivo. Questo è importante per evitare di generare istruzioni di shift con quantità di shift non valide, che potrebbero portare ad undefined behaviors.
                                if (shift_amt >= BitWidth) {
                                    mask_add.clearBit(shift_amt);
                                    continue; 
                                }
                                
                                Value *Shl;
                                // se 0 non serve creare istruzione di shift, basta usare la variabile così com'è risparmiando istruzione ulteriore
                                if (shift_amt == 0) {
                                    Shl = BestVariable;
                                } else {
                                    Value *ShiftConst = ConstantInt::get(Ty, shift_amt);
                                    Shl = Builder.CreateShl(BestVariable, ShiftConst, "naf_shl");
                                }

                                if (Accumulatore == nullptr) {
                                    Accumulatore = Shl;
                                } else {
                                    Accumulatore = Builder.CreateAdd(Accumulatore, Shl, "naf_add");
                                }
                                
                                mask_add.clearBit(shift_amt); // Spegne il bit processato (l'ultimo a destra)
                            }

                            // FASE 2: Sottrazioni, lavoriamo allo stesso modo del ciclo precedente , ma cercando numeri da sottrarre
                            APInt mask_sub = BestNM;
                            while (!mask_sub.isZero()) {
                                unsigned shift_amt = mask_sub.countr_zero();
                                
                                if (shift_amt >= BitWidth) {
                                    mask_sub.clearBit(shift_amt);
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
                                    Accumulatore = Builder.CreateNeg(Shl, "naf_neg");
                                } else {
                                    Accumulatore = Builder.CreateSub(Accumulatore, Shl, "naf_sub");
                                }
                                
                                mask_sub.clearBit(shift_amt);
                            }

                            //Se è stato inizializzato signfica che un'istruzione andrà sostituita con la serie di somme e shift
                            //procediamo a rimpiazzarne tutti gli usi con il risultato dell'accumulatore per poi eliminare l'istruzione obsoleta
                            if (Accumulatore != nullptr) {
                                Inst.replaceAllUsesWith(Accumulatore);
                                Inst.eraseFromParent();
                                Changed=true;
                                outs() << "Ottimizzazione NAF applicata con successo!\n";
                            }
                        }
                    }// <-- Chiude if (BinOp->getOpcode() == Instruction::Mul)
                else if (BinOp->getOpcode() == Instruction::SDiv || BinOp->getOpcode() == Instruction::UDiv) {
                        Value *Op0 = BinOp->getOperand(0); 
                        Value *Op1 = BinOp->getOperand(1); 

                        if (ConstantInt *CI1 = dyn_cast<ConstantInt>(Op1)) {
                            const APInt &Val1 = CI1->getValue(); //il divisore
                            Type *Ty = Inst.getType();
                            // Nessun limite di bit! Controlliamo solo che sia positivo e potenza di 2
                            if (Val1.isStrictlyPositive() && Val1.isPowerOf2()) {
                                
                                unsigned ShiftAmt = Val1.exactLogBase2();
                                
                                IRBuilder<> Builder(&Inst);
                                Value *ShiftConst = ConstantInt::get(Ty, ShiftAmt);
                                Value *NuovoShift = nullptr;

                                // Scelta tra Aritmetico (Con Segno) e Logico (Senza Segno)
                                if (BinOp->getOpcode() == Instruction::SDiv) {
                                    NuovoShift = Builder.CreateAShr(Op0, ShiftConst, "div_ashr");
                                } else {
                                    NuovoShift = Builder.CreateLShr(Op0, ShiftConst, "div_lshr");
                                }
                                //Molto semplicemente rimpiazziamo l'onerosa divisione con una shift a destra che funge da divisione per potenza di 2 e ha costo unitario.
                                Inst.replaceAllUsesWith(NuovoShift);
                                Inst.eraseFromParent();
                                Changed = true;

                                outs() << "Ottimizzazione Divisione applicata con successo!\n";
                            }
                            else if (Val1.isStrictlyPositive()) {
                                
                                // Capiamo subito se stiamo ottimizzando una SDiv o una UDiv
                                bool isSigned = (Inst.getOpcode() == Instruction::SDiv);

                                IRBuilder<> Builder(&Inst);
                                Type *Ty = Inst.getType();
                                unsigned BitWidth = Ty->getIntegerBitWidth(); 
                                Type *DoubleTy = IntegerType::get(Inst.getContext(), BitWidth * 2);

                                Value *X_ext;
                                Value *M_ext;
                                unsigned ShiftAmt;
                                bool UsaAddizioneUnsigned = false;

                                // --- FASE 1: SETUP DIFFERENZIATO (API LLVM Moderne) ---

                                if (isSigned) {
                                    // LLVM Moderno: SDiv
                                    SignedDivisionByConstantInfo MagiaS = SignedDivisionByConstantInfo::get(Val1); 
                                    ShiftAmt = MagiaS.ShiftAmount;   // L'ex "Magia.s"
                                    
                                    X_ext = Builder.CreateSExt(Op0, DoubleTy, "x_allargata_segno"); 
                                    M_ext = ConstantInt::get(DoubleTy, MagiaS.Magic.sext(BitWidth * 2)); 
                                } else {
                                    // LLVM Moderno: UDiv
                                    UnsignedDivisionByConstantInfo MagiaU = UnsignedDivisionByConstantInfo::get(Val1); 
                                    ShiftAmt = MagiaU.PostShift;         // L.ex "Magia.s"
                                    UsaAddizioneUnsigned = MagiaU.IsAdd; // L.ex "Magia.a"
                                    
                                    // ATTENZIONE: LLVM moderno a volte calcola un "Pre-Shift" per i numeri senza segno
                                    // per ottimizzare ancora di più. Dobbiamo applicarlo prima di allargare!
                                    Value *X_pre = Op0;
                                    if (MagiaU.PreShift > 0) {
                                        X_pre = Builder.CreateLShr(Op0, ConstantInt::get(Ty, MagiaU.PreShift), "pre_shift");
                                    }
                                    
                                    // Allarghiamo la X pre-shiftata invece di quella originale
                                    X_ext = Builder.CreateZExt(X_pre, DoubleTy, "x_allargata_zeri"); 
                                    M_ext = ConstantInt::get(DoubleTy, MagiaU.Magic.zext(BitWidth * 2)); // L'ex "Magia.m"
                                }

                                // --- FASE 2: TRONCO COMUNE (Moltiplicazione e Taglio) ---
                                // Queste 3 istruzioni sono identiche per entrambi i tipi di divisione
                                Value *Mul = Builder.CreateMul(X_ext, M_ext, "moltiplicazione_magica");
                                
                                // Spostiamo la metà alta verso il basso (qui va sempre usato LShr!)
                                Value *ShiftAlta = Builder.CreateLShr(Mul, ConstantInt::get(DoubleTy, BitWidth));
                                
                                // Tagliamo via i 32 bit eccedenti per tornare alla dimensione originale
                                Value *Risultato = Builder.CreateTrunc(ShiftAlta, Ty, "parte_alta_32bit");


                                // --- FASE 3: CEROTTI DIFFERENZIATI E SHIFT FINALE ---
                                Value *RisultatoFinale;

                                if (isSigned) {
                                    // A) Shift finale ARITMETICO (AShr) per preservare i numeri negativi
                                    Value *DivShift = Builder.CreateAShr(Risultato, ConstantInt::get(Ty, ShiftAmt), "shift_aritmetico_finale");
                                    
                                    // B) Cerotto SDiv: Estraiamo il bit di segno originale e lo sommiamo
                                    // Questo corregge l'arrotondamento verso il basso dello shift trasformandolo in arrotondamento verso zero
                                    Value *SignShiftAmt = ConstantInt::get(Ty, BitWidth - 1);
                                    Value *BitDiSegno = Builder.CreateLShr(Op0, SignShiftAmt, "estrai_bit_segno");
                                    RisultatoFinale = Builder.CreateAdd(DivShift, BitDiSegno, "risultato_sdiv_corretto");
                                    
                                } else {
                                    // A) Cerotto UDiv: Applichiamo la formula di compensazione matematica se richiesta da LLVM
                                    if (UsaAddizioneUnsigned) {
                                        Value *Sub = Builder.CreateSub(Op0, Risultato);
                                        Value *Shr1 = Builder.CreateLShr(Sub, ConstantInt::get(Ty, 1));
                                        Risultato = Builder.CreateAdd(Risultato, Shr1, "addizione_compensativa");
                                    }
                                    
                                    // B) Shift finale LOGICO (LShr) per i numeri senza segno
                                    RisultatoFinale = Builder.CreateLShr(Risultato, ConstantInt::get(Ty, ShiftAmt), "risultato_udiv_corretto");
                                }

                                // --- FASE 4: SOSTITUZIONE ---
                                // Sostituiamo la vecchia divisione hardware lentissima con la nostra cascata di shift
                                Inst.replaceAllUsesWith(RisultatoFinale);
                                Inst.eraseFromParent();
                                Changed=true;
                                outs() << "Ottimizzazione Divisione applicata con successo!\n";
                            }
                        } // chiude if (ConstantInt *CI1 = dyn_cast<ConstantInt>(Op1))

                    } // <--- PARENTESI AGGIUNTA: Chiude correttamente l'else if della divisione
                } // <-- Chiude if (auto *BinOp = dyn_cast<BinaryOperator>(&Inst))
            } // <-- Chiude for (auto IterINSTR = B.begin(); ...)
        } // <-- Chiude for (auto IterBB = F.begin(); ...)
        if(Changed) {
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

// MultiInstructionOptimization implementation
struct MultiInstructionOptimization: PassInfoMixin<MultiInstructionOptimization> {
  // Main entry point, takes IR unit to run the pass on (&F) and the
  // corresponding pass manager (to be queried if need be)

    /** Riconosce gli operandi in operazioni in cui vale la proprietà commutativa.
     *  Restituisce un std::pair in cui first è un'istruzione e second è il valore costante.
     * */
    std::pair<Value*, ConstantInt*> parseCommutative(const BinaryOperator* op){

        std::pair<Value*, ConstantInt*> operands = {nullptr, nullptr};
        auto lhsConst = dyn_cast<ConstantInt>(op->getOperand(0));
        auto rhsConst = dyn_cast<ConstantInt>(op->getOperand(1));

        // Se nessuno degli operandi è costante o lo sono entrambi, l'addizione non è interessata dall'ottimizzazione
        if (lhsConst == nullptr && rhsConst != nullptr)
            operands = {op->getOperand(0), rhsConst};
        else if (lhsConst != nullptr && rhsConst == nullptr)
            operands = {op->getOperand(1), lhsConst};
        return operands;
    }

    // Restituisce l'operazione opposta a quella passata per argomento
    Instruction::BinaryOps getOppositeOpcode(Instruction::BinaryOps opcode){
        switch (opcode){
            case Instruction::Add:
                return Instruction::Sub;
            case Instruction::Sub:
                return Instruction::Add;
            case Instruction::Mul:
                return Instruction::UDiv;
            case Instruction::UDiv:
            case Instruction::SDiv:
                return Instruction::Mul;
        }
        return opcode;
    }

    PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
        bool changed = false;
        for (auto BBiter = F.begin(); BBiter != F.end(); ++BBiter){
            BasicBlock &BB = *BBiter;            
            for (auto Iiter = BB.begin(); Iiter != BB.end(); ++Iiter){
                Instruction &I = *Iiter;
                auto binOp = dyn_cast<BinaryOperator>(&I);
                std::pair<Value*, ConstantInt*> commutativeOperands = {nullptr, nullptr};
                ConstantInt* firstConstOperand = nullptr;
                if (binOp == nullptr)
                    continue;

                // Se l'operatore gode della proprietà commutativa, identifica l'operando costante e l'operando istruzione indipendentemente dall'ordine in cui si presentano nell'operazione
                if (binOp->getOpcode() == Instruction::Add || binOp->getOpcode() == Instruction::Mul){
                    commutativeOperands = parseCommutative(binOp);
                    if (commutativeOperands.first == nullptr || commutativeOperands.second == nullptr)
                        continue;
                    // ora anche firstConstOperand punta al valore costante della prima istruzione 
                    firstConstOperand = commutativeOperands.second;
                }

                // Se la proprietà commutativa non si applica all'operatore si considerano solo le istruzioni in cui il valore costante è a destra
                else if (binOp->getOpcode() == Instruction::Sub || binOp->getOpcode() == Instruction::UDiv){
                    firstConstOperand = dyn_cast<ConstantInt>(binOp->getOperand(1));
                    if (firstConstOperand == nullptr)
                        continue;
                }

                // Itera tutti gli user dell'istruzione
                for (auto Uiter = I.user_begin(); Uiter != I.user_end();){
                    //Instruction *user = dyn_cast<Instruction>(*Uiter);
                    User* user = *Uiter;
                    Uiter++;
                    if (user == nullptr)
                        continue;
                    const BinaryOperator* userBinOp = dyn_cast<BinaryOperator>(user);

                    std::pair<Value*, ConstantInt*> userCommutativeOperands = {nullptr, nullptr};
                    ConstantInt* userConstOperand = nullptr;

                    if (userBinOp == nullptr)
                        continue;

                    // Se l'operatore dello user non è "opposto" all'operatore dell'istruzione che si sta analizzando l'istruzione user non è interessata dalla ottimizzazione
                    if (userBinOp->getOpcode() != getOppositeOpcode(binOp->getOpcode()))
                        continue;
                    
                    // Se user è commutativa ordina gli operandi nella forma {istruzione, costante} passandola a parseCommutative
                    if (userBinOp->getOpcode() == Instruction::Add || userBinOp->getOpcode() == Instruction::Mul){
                        userCommutativeOperands = parseCommutative(userBinOp);
                        // Se dopo il casting avvenuto in parseCommutative un operando è nullo l'istruzione user non è interessata dall'ottimizzazione
                        if (userCommutativeOperands.first == nullptr || userCommutativeOperands.second == nullptr)
                            continue;
                        // ora anche userConstOperand punta al valore costante dell'istruzione user
                        userConstOperand = userCommutativeOperands.second;
                    } 
                    // Se user non è commutativa cerca il valore costante solo a destra
                    else if (userBinOp->getOpcode() == Instruction::Sub || userBinOp->getOpcode() == Instruction::UDiv){
                        userConstOperand = dyn_cast<ConstantInt>(userBinOp->getOperand(1));
                        // Se il casting a ConstantInt dell'operando destro non ha successo significa che l'istruzione di cui fa parte non è interessata dall'ottimizzazione
                        if (userConstOperand == nullptr)
                            continue;
                    }
                    
                    // Se i valori costanti riconosciuti nelle operazioni "opposte" sono uguali tra loro si applica l'ottimizzazione
                    if (userConstOperand->getZExtValue() == firstConstOperand->getZExtValue()){
                        if (binOp->getOpcode() == Instruction::Add || binOp->getOpcode() == Instruction::Mul){
                            user->replaceAllUsesWith(commutativeOperands.first);
                        }
                        else if (binOp->getOpcode() == Instruction::Sub || binOp->getOpcode() == Instruction::UDiv)
                            user->replaceAllUsesWith(binOp->getOperand(0));
                        changed = true;
                        Instruction *userInst = dyn_cast<Instruction>(user);
                        userInst->dropAllReferences();
                        userInst->eraseFromParent();
                    }
                }
            }
        }
        if(changed) {
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
              if (Name == "str-red-pass") {
                FPM.addPass(StrengthReductionPass());
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
	              FPM.addPass(StrengthReductionPass());
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

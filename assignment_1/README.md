# First Assignment

## Overview 

In questo assignment sono stati implementati 3 passi di ottimizzazione LLVM:

1. **Algebraic Identity**  
    - Semplificazione di espressioni algebriche:  
        - `𝑥 + 0 = 0 + 𝑥 ⇒ 𝑥`
        - `𝑥 × 1 = 1 × 𝑥 ⇒ 𝑥`
        - `(extra): 𝑥 - 0 ⇒ 𝑥`
        - `(extra): 𝑥 × 0 = 0 × 𝑥 ⇒ 0`
        - `(extra): x / 1 ⇒ x`
        - `(extra): x << 0 = x >> 0 ⇒ x`

2. **Strength Reduction** 
    - Sostituzione di operazioni costose con operazioni equivalenti più efficienti:  
        - `15 × 𝑥 ⇒ (𝑥 ≪ 4) – 𝑥`  
        - `𝑦 = 𝑥 / 8 ⇒ 𝑦 = 𝑥 >> 3`  

3. **Multi-Instruction Optimization**  
    - Ottimizzazione di sequenze di istruzioni ridondanti:  
        - `𝑎 = 𝑏 + 1, 𝑐 = 𝑎 − 1 ⇒ 𝑎 = 𝑏 + 1, 𝑐 = 𝑏`
        - `(extra): 𝑎 = 2 * b, 𝑐 = 𝑎 / 2 ⇒ 𝑎 = 2 * b, 𝑐 = 𝑏`

---

## Directory Structure

- La cartella `SRC/`: contiene i file dell'iimplementazione dei passi.
- La cartella `TEST/`: contiene i file di test per le varie ottimizzazioni.
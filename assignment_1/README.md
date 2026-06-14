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
        - `15 × 𝑥 ⇒ (𝑥 << 4) – 𝑥`  
        - `𝑦 = 𝑥 / 8 ⇒ 𝑦 = 𝑥 >> 3`
        - `(extra): -3 × 𝑥 ⇒ x - (𝑥 << 2)`

3. **Multi-Instruction Optimization**  
    - Ottimizzazione di sequenze di istruzioni ridondanti:  
        - `𝑎 = 𝑏 + 1, 𝑐 = 𝑎 − 1 ⇒ 𝑎 = 𝑏 + 1, 𝑐 = 𝑏`
        - `(extra): 𝑎 = 2 * b, 𝑐 = 𝑎 / 2 ⇒ 𝑎 = 2 * b, 𝑐 = 𝑏`

## Usage

Per testare i passi di ottimizzazione bisogna fare i seguenti comandi:
1. `export LLVM_DIR=<installation/dir/of/llvm/19>/bin`
2. `./build.sh`
3. `./test.sh`

Una volta che i due script hanno finito si possono visionare i risultati guardando il file `output.log`

---

## Directory Structure

- La cartella `SRC/`: contiene i file dell'implementazione dei passi.
- La cartella `TEST/`: contiene i file di test per le varie ottimizzazioni. E' suddivisa in:
    - `TEST/Alg_id_TEST/`: al suo interno ci sono i file di test per il passo Algebraic Identity
    - `TEST/Multi_inst_opt_TEST/`: al suo interno ci sono i file di test per il passo Multi-Instruction Optimization
    - `TEST/Str_red_TEST/`: al suo interno ci sono i file di test per il passo Strength Reduction

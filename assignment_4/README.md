# Fourth Assignment

## Overview 

In questo assignment è stato implementato il passo di ottimizzazione della Loop Fusion. Questa ottimizzazione verifica se due loop all'interno del CFG si possano fondere in modo da avere delle ottimizzazioni sulla memoria.

## Requirements

Non tutti i loop si possono fondere bisogna che siano rispettate le seguenti condizioni:
1. **Adiacenza**  
    - I due loop devono essere adiacenti tra di loro, non ci devono essere istruzioni in mezzo (cioè tra la fine del primo loop e l'inizio del secondo loop)
    - I due loop devono avere la stessa semantica quindi entrambi guarded oppure entrambi unguarded
2. **Stesso numero di iterazioni**  
    - Bisogna controllare che entrambi i loop facciano lo stesso numero di iterazioni
3. **Equivalenza del Control Flow**  
    - Bisogna controllare la dominanza e la post dominanza dei due loop
4. **Nessuna dipendenza negativa**  
    - Bisogna controllare che non ci siano dipendenze negative, ad esempio una dipedenza negativa accade quando il secondo loop usa un valore calcolato dal primo loop in un iterazione futura

Una volta effettuati questi accertamenti per i due loop, presi in considerazione, si può passare a fare la fusione 

## Testing

Per testare i passi di ottimizzazione si può eseguire lo script test.sh a cui si devono passare come argomenti:
1. Directory in cui sono contenuti i file .cpp e/o .ll
2. Directory in cui è contenuto il file .so del passo
3. Nome del file su cui salvare l'output del passo per ogni file su cui viene eseguito

---

## Directory Structure

- La cartella `SRC/`: contiene i file dell'implementazione del passo.
- La cartella `TEST/`: contiene i file di test. E' suddivisa in:
    - `TEST/Edges_TEST/`: al suo interno ci sono i file di test che non passano tutti i controlli
    - `TEST/Successful_TEST/`: al suo interno ci sono i file di test che passano tutti i controlli
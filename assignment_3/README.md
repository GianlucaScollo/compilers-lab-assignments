# Third Assignment

## Overview 

In questo assignment è stato implementato il passo di ottimizzazione della Loop-Invariant Code Motion (LICM). Questa ottimizzazione identifica le istruzioni che sono loop-invariant, dentro il loop, e verifica se si possono spostare nel blocco preheader del loop; in questo modo durante l'esecuzione del loop non verrano rifatti calcoli ridondanti (che potrebbero essere costosi) riuscendo così a migliorare le prestazioni del programma.

## Requirements

Un'istruzione è considerata loop-invariant se tutti i suoi operandi sono:
- delle costanti
- argomenti di funzione
- valori definiti fuori dal loop
- oppure se usa degli operandi di istruzioni dentro il loop che sono state marchiate loop-invariant

Un'istruzione anche se considerata loop-invariant non è detto che si possa poi spostare, infatti deve rispettare le seguenti condizioni:
- non deve causare side effects, quindi deve essere safe da eseguire
- deve dominare tutti gli usi all'interno del loop, quindi nel caso in cui venisse spostata non deve cambiare la semantica del programma

---

## Directory Structure

- La cartella `SRC/`: contiene i file dell'implementazione del passo.
- La cartella `TEST/`: contiene i file di test.
# compiles-lab-assignments

**Author:** Benassi Lorenzo (187849), Borghi Emanuele (187264), Scollo Gianluca (187575)

---

## Overview  
Questo repository contiene i quattro assignment del secondo modulo del corso di Laboratori di Compilatori.

---

## Repository Structure

```text
root/
├── assignment_1/                   # First assignment: implementazione di 3 passi di ottimizzazione
│   ├── SRC/                        # Cartella con i file sorgenti
│   │   ├── CMakeLists.txt          # File di configurazione per il comando cmake
│   │   └── FirstAssignment.cpp     # Implementazione del passo di ottimizzazione 
│   ├── TEST/                       # Cartella contenente i vari file di test
│   │   ├── Alg_id_TEST/            # Cartella con i file di test per il passo Algebraic Identity
│   │   ├── Multi_inst_opt_TEST/    # Cartella con i file di test per il passo Multi-Instruction Optimization
│   │   └── Str_red_TEST/           # Cartella con i file di test per il passo Strength Reduction
│   ├── build.sh                    # Script per fare il build del passo
│   ├── output.log                  # Risultato dell'esecuzione dello script test.sh
│   ├── README.md                   # Descrizione dell'assignment
│   └── test.sh                     # Script per ottenere i file di test ottimizzati
│
├── assignment_2/                   # Second assignment: Dataflow Analysis di 3 problemi
│   └── Dataflow_Analysis.pdf       # File con le analisi
│
├── assignment_3/                   # Third assignment: implementazione del Loop-Invariant Code Motion
│   ├── SRC/                        # Cartella con i file sorgenti
│   │   ├── CMakeLists.txt          # File di configurazione per il comando cmake
│   │   └── ThirdAssignment.cpp     # Implementazione del passo di ottimizzazione 
│   ├── TEST/                       # Cartella contenente i vari file di test
│   │   └── ...                     # Test vari
│   ├── build.sh                    # Script per fare il build del passo
│   ├── output.log                  # Risultato dell'esecuzione dello script test.sh
│   ├── README.md                   # Descrizione dell'assignment
│   └── test.sh                     # Script per ottenere i file di test ottimizzati
│
├── assignment_4/                   # Fourth assignment: implementazione della Loop Fusion
│   ├── SRC/                        # Cartella con i file sorgenti
│   │   ├── CMakeLists.txt          # File di configurazione per il comando cmake
│   │   └── FourthAssignment.cpp    # Implementazione del passo di ottimizzazione 
│   ├── TEST/                       # Cartella contenente i vari file di test
│   │   ├── Edges_TEST/             # Cartella con i file di test che non passano tutti i controlli
│   │   └── Successful_TEST/        # Cartella con i file di test che passano tutti i controlli
│   ├── build.sh                    # Script per fare il build del passo
│   ├── output.log                  # Risultato dell'esecuzione dello script test.sh
│   ├── README.md                   # Descrizione dell'assignment
│   └── test.sh                     # Script per ottenere i file di test ottimizzati
├── .gitignore                      # gitignore file
└── README.md                       # Attualmente ti trovi qui
```
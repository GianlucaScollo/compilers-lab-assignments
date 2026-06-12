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
│   │   ├── CMakeLists.txt
│   │   └── FirstAssignment.cpp
│   ├── TEST/                       # Cartella contenente i vari file di test
│   │   ├── Alg_id_TEST/            # Cartella con i file di test per il passo Algebraic Identity
│   │   ├── Multi_inst_opt_TEST/    # Cartella con i file di test per il passo Multi-Instruction Optimization
│   │   └── Str_red_TEST/           # Cartella con i file di test per il passo Strength Reduction
│   └── README.md
├── assignment_2/                   # Second assignment: Dataflow Analysis di 3 problemi
│   └── Dataflow_Analysis.pdf       # File con le analisi
├── assignment_3/                   # Third assignment: implementazione del Loop-Invariant Code Motion
│   ├── SRC/                        # Cartella con i file sorgenti
│   │   ├── CMakeLists.txt
│   │   └── ThirdAssignment.cpp
│   ├── TEST/                       # Cartella contenente i vari file di test
│   │   └── ...
│   └── README.md
├── assignment_4/                   # Fourth assignment: implementazione della Loop Fusion
│   ├── SRC/                        # Cartella con i file sorgenti
│   │   ├── CMakeLists.txt
│   │   └── FourthAssignment.cpp
│   ├── TEST/                       # Cartella contenente i vari file di test
│   │   ├── Edges_TEST/             # Cartella con i file di test che non passano tutti i controlli
│   │   └── Successful_TEST/        # Cartella con i file di test che passano tutti i controlli
│   └── README.md
├── .gitignore                      # gitignore file
└── README.md                       # Attualmente ti trovi qui
```
#!/bin/bash

BUILD_DIR="${1:-BUILD}"

echo "Inizializzazione build LLVM..."

# Controlla se la directory build esiste, se no la crea
if [[ ! -d "$BUILD_DIR" ]]; then
    echo "Creazione directory build..."
    mkdir -p "$BUILD_DIR"
fi

# Entra nella directory build
cd "$BUILD_DIR"

# Controllo se CMake è già stato configurato (quindi se è già stato fatto il cmake)
if [[ ! -f "CMakeCache.txt" ]]; then
    echo "Configurazione CMake..."

    if [[ -z "$LLVM_DIR" ]]; then
        echo "[ERRORE] LLVM_DIR non impostata"
        exit 1
    fi

    if ! cmake -DLT_LLVM_INSTALL_DIR="$LLVM_DIR" ../SRC/; then
        echo "Errore durante il cmake"
        cd ..
        exit 1
    fi
fi

echo "Compilazione del progetto..."

# Compila il progetto
if ! make; then
    echo "Errore durante il make"
    cd ..
    exit 1
fi

cd ..
echo "Compilazione completata con successo"
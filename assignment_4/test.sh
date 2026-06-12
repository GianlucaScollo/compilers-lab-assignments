#!/bin/bash

# Usage: ./run_pipeline.sh <TEST_DIR> <BUILD_DIR> <OUTPUT_FILE>
# Example: ./run_pipeline.sh ./TEST ./BUILD output.log

TEST_DIR="${1:-TEST}"
BUILD_DIR="${2:-BUILD}"
OUTPUT_FILE="${3:-output.log}"
SEPARATOR="$(printf '=%.0s' {1..60})"

# Svuota o crea il file di output
> "$OUTPUT_FILE"

# Controlla che la directory TEST esista
if [[ ! -d "$TEST_DIR" ]]; then
    echo "Errore: la directory '$TEST_DIR' non esiste."
    exit 1
fi

# Cerca tutti i file .cpp nella directory TEST
shopt -s nullglob
files=("$TEST_DIR"/*.cpp)

if [[ ${#files[@]} -eq 0 ]]; then
    echo "Nessun file .cpp trovato in '$TEST_DIR'."
    exit 1
fi

for filepath in "${files[@]}"; do
    filename="$(basename "$filepath")"          # nomefile.cpp
    base="${filename%.cpp}"                     # nomefile

    ll="$TEST_DIR/$base.ll"
    m2r="$TEST_DIR/$base.m2r.ll"
    opt_out="$TEST_DIR/$base.optimized.ll"

    echo "$SEPARATOR" >> "$OUTPUT_FILE"
    echo "FILE: $filename" >> "$OUTPUT_FILE"
    echo "$SEPARATOR" >> "$OUTPUT_FILE"

    # Step 1: compilazione con clang
    echo "--- clang ---" >> "$OUTPUT_FILE"
    clang -O0 -emit-llvm -S -c -Xclang -disable-O0-optnone \
        "$filepath" -o "$ll" >> "$OUTPUT_FILE" 2>&1
    if [[ $? -ne 0 ]]; then
        echo "[ERRORE] clang fallito per $filename" >> "$OUTPUT_FILE"
        continue
    fi

    # Step 2: mem2reg
    echo "--- opt mem2reg ---" >> "$OUTPUT_FILE"
    opt -passes=mem2reg "$ll" -S -o "$m2r" >> "$OUTPUT_FILE" 2>&1
    if [[ $? -ne 0 ]]; then
        echo "[ERRORE] opt mem2reg fallito per $filename" >> "$OUTPUT_FILE"
        continue
    fi

    # Step 3: plugin personalizzato
    echo "--- opt lf (plugin) ---" >> "$OUTPUT_FILE"
    opt -S -load-pass-plugin "$BUILD_DIR/libFourthAssignment.so" \
        -passes=lf "$m2r" -o "$opt_out" >> "$OUTPUT_FILE" 2>&1
    if [[ $? -ne 0 ]]; then
        echo "[ERRORE] opt lf fallito per $filename" >> "$OUTPUT_FILE"
        continue
    fi

    echo "[OK] $filename processato con successo" >> "$OUTPUT_FILE"
done

echo "$SEPARATOR" >> "$OUTPUT_FILE"
echo "Pipeline completata. Output salvato in: $OUTPUT_FILE"
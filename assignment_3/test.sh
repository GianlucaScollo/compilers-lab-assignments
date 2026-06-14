#!/bin/bash

# Usage: ./test.sh <TEST_DIR> <BUILD_DIR> <OUTPUT_FILE>
# Default: ./test.sh (it will use: TEST BUILD output.log)

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

# Cerca tutti i file .cpp nella directory TEST e nelle sue sottocartelle
mapfile -d '' files < <(find "$TEST_DIR" -type f -name '*.cpp' -print0)

if [[ ${#files[@]} -eq 0 ]]; then
    echo "Nessun file .cpp trovato in '$TEST_DIR' o nelle sue sottocartelle."
    exit 1
fi

for filepath in "${files[@]}"; do
    filename="$(basename "$filepath")"          # nomefile.cpp
    base="${filename%.cpp}"                     # nomefile
    rel_dir="$(dirname "${filepath#$TEST_DIR/}")"

    if [[ "$rel_dir" == "." ]]; then
        out_dir="$TEST_DIR"
    else
        out_dir="$TEST_DIR/$rel_dir"
    fi

    ll="$out_dir/$base.ll"
    m2r="$out_dir/$base.m2r.ll"
    opt_out="$out_dir/$base.optimized.ll"

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
    opt -S -load-pass-plugin "$BUILD_DIR/libThirdAssignment.so" \
        -passes=lf "$m2r" -o "$opt_out" >> "$OUTPUT_FILE" 2>&1
    if [[ $? -ne 0 ]]; then
        echo "[ERRORE] opt lf fallito per $filename" >> "$OUTPUT_FILE"
        continue
    fi

    echo "[OK] $filename processato con successo" >> "$OUTPUT_FILE"
done

echo "$SEPARATOR" >> "$OUTPUT_FILE"
echo "Pipeline completata. Output salvato in: $OUTPUT_FILE"

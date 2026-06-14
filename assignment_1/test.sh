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

# Controlla che la directory BUILD esista
if [[ ! -d "$BUILD_DIR" ]]; then
    echo "Errore: la directory '$BUILD_DIR' non esiste."
    exit 1
fi

# Controllo che il plugin esista
if [[ ! -f "$BUILD_DIR/libFirstAssignment.so" ]]; then
    echo "Errore: Plugin non trovato: $BUILD_DIR/libFirstAssignment.so"
    exit 1
fi

# Cerca tutti i file .ll nella directory TEST e nelle sue sottocartelle
mapfile -d '' files < <(find "$TEST_DIR" -type f -name '*.ll' -print0)

if [[ ${#files[@]} -eq 0 ]]; then
    echo "Nessun file .ll trovato in '$TEST_DIR' o nelle sue sottocartelle."
    exit 1
fi

for filepath in "${files[@]}"; do
    filename="$(basename "$filepath")"               # nomefile.ll
    base="${filename%.ll}"                           # nomefile
    rel_dir="$(dirname "${filepath#$TEST_DIR/}")"

    if [[ "$rel_dir" == "." ]]; then
        out_dir="$TEST_DIR"
    else
        out_dir="$TEST_DIR/$rel_dir"
    fi

    ll="$out_dir/$base.ll"
    opt_out="$out_dir/$base.optimized.ll"

    echo "$SEPARATOR" >> "$OUTPUT_FILE"
    echo "FILE: $filename" >> "$OUTPUT_FILE"
    echo "$SEPARATOR" >> "$OUTPUT_FILE"

    if [[ "$rel_dir" == "Alg_id_TEST" ]]; then
        PASS="alg-id"
    elif [[ "$rel_dir" == "Multi_inst_opt_TEST" ]]; then
        PASS="mul-ins-opt"
    elif [[ "$rel_dir" == "Str_red_TEST" ]]; then
        PASS="str-red-pass"
    else
        echo "[ERRORE] Cartella non riconosciuta" >> "$OUTPUT_FILE"
        continue
    fi

    # Step plugin personalizzato
    echo "--- opt $PASS (plugin) ---" >> "$OUTPUT_FILE"
    opt -S -load-pass-plugin "$BUILD_DIR/libFirstAssignment.so" \
        -passes="$PASS" "$ll" -o "$opt_out" >> "$OUTPUT_FILE" 2>&1
    if [[ $? -ne 0 ]]; then
        echo "[ERRORE] opt $PASS fallito per $filename" >> "$OUTPUT_FILE"
        continue
    fi

    echo "[OK] $filename processato con successo" >> "$OUTPUT_FILE"
done

echo "$SEPARATOR" >> "$OUTPUT_FILE"
echo "Pipeline completata. Output salvato in: $OUTPUT_FILE"

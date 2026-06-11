; int moltiplica_per_meno_3(int x) {
;   int mul = x * (-3);
;   return mul;
; }

; Funzione 2: Moltiplica per -3 (Per testare se il tuo controllo del segno funziona)
define dso_local i32 @moltiplica_per_meno_3(i32 %x) {
entry:
  %mul = mul nsw i32 %x, -3
  ret i32 %mul
}
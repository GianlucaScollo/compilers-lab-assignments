define dso_local i32 @moltiplica_per_15(i32 %x) {
entry:
  ; Questa è l'istruzione esatta che il tuo pass cercherà e trasformerà!
  ; (nsw significa "No Signed Wrap", LLVM sa che non dovrebbe andare in overflow)
  %mul = mul nsw i32 %x, 15
  ret i32 %mul
}
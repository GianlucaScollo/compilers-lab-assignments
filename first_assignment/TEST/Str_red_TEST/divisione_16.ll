define dso_local i128 @divisione_16(i128 %x) {
entry:
  ; Questa è l'istruzione esatta che il pass cercherà e trasformerà!
  ; (nsw significa "No Signed Wrap", LLVM sa che non dovrebbe andare in overflow)
  %div = sdiv i128 %x, 16 
  ret i128 %div
}
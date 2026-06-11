; int moltiplica_per_15(int x) {
;   int mul = x * 15;
;   return mul;
; }

define dso_local i32 @moltiplica_per_15(i32 %x) {
entry:
  %mul = mul nsw i32 %x, 15
  ret i32 %mul
}
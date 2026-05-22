; int mul_131(__int128 x) {
;   __int128 mul = 131 * x;
;   return mul;
; }

define dso_local i128 @mul_131(i128 %x) {
entry:
  %mul = mul nsw i128 131, %x
  ret i128 %mul
}
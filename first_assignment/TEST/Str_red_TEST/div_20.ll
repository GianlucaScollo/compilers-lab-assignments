; int divisione_20(__int128 x) {
;   __int128 div = x / 20;
;   return div;
; }

define dso_local i128 @divisione_20(i128 %x) {
entry:
  %div = sdiv i128 %x, 20 
  ret i128 %div
}
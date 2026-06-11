; int divisione_16(__int128 x) {
;   __int128 div = x / 16;
;   return div;
; }

define dso_local i128 @divisione_16(i128 %x) {
entry:
  %div = sdiv i128 %x, 16 
  ret i128 %div
}
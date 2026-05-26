; int foo(int x) {
;   int a = x << 0;     // shl  (shift sinistro)
;   int b = x >> 0;     // lshr (shift destro logico usato per interi unsigned)
;   int c = x >> 0;     // ashr (shift destro aritmetico usato per interi signed)
;   return a + b + c;
; }

define dso_local i32 @foo(i32 noundef %x) {
entry:
  %a = shl i32 %x, 0
  %b = lshr i32 %x, 0
  %c = ashr i32 %x, 0
  %t0 = add i32 %a, %b
  %t1 = add i32 %t0, %c
  ret i32 %t1
}
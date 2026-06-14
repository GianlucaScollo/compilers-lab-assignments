; int all(int x) {
;   int a = x + 0;
;   int b = 0 + x;
;   int c = x - 0;
;   int d = x * 1;
;   int e = 1 * x;
;   int f = x / 1;              // sdiv
;   int g = (unsigned)x / 1;    // udiv
;   int h = x << 0;             // shl
;   int i = x >> 0;             // logical shr
;   int j = x >> 0;             // arithmetic shr
;   return a+b+c+d+e+f+g+h+i+j;
; }

define dso_local i32 @all(i32 noundef %x) {
entry:
  %a = add i32 %x, 0
  %b = add i32 0, %x
  %c = sub i32 %x, 0
  %d = mul i32 %x, 1
  %e = mul i32 1, %x
  %f = sdiv i32 %x, 1
  %g = udiv i32 %x, 1
  %h = shl  i32 %x, 0
  %i = lshr i32 %x, 0
  %j = ashr i32 %x, 0

  %t0 = add i32 %a, %b
  %t1 = add i32 %t0, %c
  %t2 = add i32 %t1, %d
  %t3 = add i32 %t2, %e
  %t4 = add i32 %t3, %f
  %t5 = add i32 %t4, %g
  %t6 = add i32 %t5, %h
  %t7 = add i32 %t6, %i
  %t8 = add i32 %t7, %j
  ret i32 %t8
}
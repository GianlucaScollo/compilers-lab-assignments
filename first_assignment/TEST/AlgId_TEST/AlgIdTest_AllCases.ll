; int all(int x) {
;   int a = x + 0;
;   int b = 0 + x;
;   int c = x - 0;
;   int d = x * 1;
;   int e = 1 * x;
;   int f = x * 0;
;   int g = 0 * x;
;   int h = x / 1;              // sdiv
;   int i = (unsigned)x / 1;    // udiv
;   int j = x << 0;             // shl
;   int k = x >> 0;             // logical shr
;   int m = x >> 0;             // arithmetic shr
;   return a+b+c+d+e+f+g+h+i+j+k+m;
; }

define dso_local i32 @all(i32 noundef %x) {
entry:
  %a = add i32 %x, 0
  %b = add i32 0, %x
  %c = sub i32 %x, 0
  %d = mul i32 %x, 1
  %e = mul i32 1, %x
  %f = mul i32 %x, 0
  %g = mul i32 0, %x
  %h = sdiv i32 %x, 1
  %i = udiv i32 %x, 1
  %j = shl  i32 %x, 0
  %k = lshr i32 %x, 0
  %m = ashr i32 %x, 0

  %t0 = add i32 %a, %b
  %t1 = add i32 %t0, %c
  %t2 = add i32 %t1, %d
  %t3 = add i32 %t2, %e
  %t4 = add i32 %t3, %f
  %t5 = add i32 %t4, %g
  %t6 = add i32 %t5, %h
  %t7 = add i32 %t6, %i
  %t8 = add i32 %t7, %j
  %t9 = add i32 %t8, %k
  %tA = add i32 %t9, %m
  ret i32 %tA
}
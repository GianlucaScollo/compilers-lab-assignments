; int foo(int b) {
;   int a = 2 * b;
;   int c = a / 2;
;   return c;
; }

define dso_local i32 @foo(i32 noundef %0) #0 {
   %2 = mul nsw i32 2, %0
   %3 = udiv i32 %2, 2
   ret i32 %3
}

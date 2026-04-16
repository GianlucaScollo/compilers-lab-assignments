; int foo(int e, int a) {
;   int b = a + 0;
;   int c = 1 * e;
;   int d = b - c;
;   return d;
; }

define dso_local i32 @foo(i32 noundef %0, i32 noundef %1) #0 {
   %3 = add nsw i32 %1, 0
   %4 = mul nsw i32 1, %0
   %5 = sub nsw i32 %3, %4
   ret i32 %5
}

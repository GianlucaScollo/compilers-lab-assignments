; int foo(int e, int a) {
;   int b = a + 0;
;   int d = b * 1;
;   return d;
; }

define dso_local i32 @foo(i32 noundef %0, i32 noundef %1) #0 {
   %3 = add nsw i32 %1, 0
   %4 = mul nsw i32 %3, 1
   ret i32 %4
}

; int foo(int e, int a) {
;   int b = 0 + 0;
;   int d = b;
;   return d;
; }

define dso_local i32 @foo(i32 noundef %0, i32 noundef %1) #0 {
   %3 = add nsw i32 0, 0
   %4 = add nsw i32 %3, 0
   ret i32 %4
}

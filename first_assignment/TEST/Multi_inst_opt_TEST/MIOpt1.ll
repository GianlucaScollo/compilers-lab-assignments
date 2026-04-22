; int foo(int d, int b) {
;   int a = b + 1;
;   int c = a - 1;
;   return c;
; }

define dso_local i32 @foo(i32 noundef %0, i32 noundef %1) #0 {
   %3 = add nsw i32 %1, 1
   %4 = sub nsw i32 %3, 1
   ret i32 %4
}

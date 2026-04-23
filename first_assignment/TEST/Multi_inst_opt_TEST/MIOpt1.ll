; int foo(int b) {
;   int a = b - 5;
;   int c = 5 + a;
;   return c;
; }

define dso_local i32 @foo(i32 noundef %0) #0 {
   %2 = sub nsw i32 %0, 5
   %3 = add nsw i32 5, %2
   ret i32 %3
}

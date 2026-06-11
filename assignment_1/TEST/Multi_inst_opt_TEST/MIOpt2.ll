; int foo(int b) {
;   int a = b + 1;
;   int c = a - 1;
;   return c;
; }

define dso_local i32 @foo(i32 noundef %0) #0 {
   %2 = add nsw i32 %0, 1
   %3 = sub nsw i32 %2, 1
   ret i32 %3
}

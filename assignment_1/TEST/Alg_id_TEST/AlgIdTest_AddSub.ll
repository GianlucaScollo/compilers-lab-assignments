; int foo(int x) {
;   int a = x + 0;
;   int b = 0 + a;
;   int c = b - 0;
;   return c;
; }

define dso_local i32 @foo(i32 noundef %x) {
entry:
    %a = add nsw i32 %x, 0
    %b = add nsw i32 0, %a
    %c = sub nsw i32 %b, 0
    ret i32 %c
}
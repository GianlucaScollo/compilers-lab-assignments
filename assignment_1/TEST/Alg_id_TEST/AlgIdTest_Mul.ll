; int foo(int x) {
;   int a = x * 1;
;   int b = 1 * a;
;   return a + b;
; }

define dso_local i32 @foo(i32 noundef %x) {
entry:
    %a = mul nsw i32 %x, 1
    %b = mul nsw i32 1, %a
    %t0 = add nsw i32 %a, %b
    ret i32 %t0
}
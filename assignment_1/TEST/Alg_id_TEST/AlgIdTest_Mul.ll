; int foo(int x) {
;   int a = x * 1;
;   int b = 1 * a;
;   int c = b * 0;
;   int d = 0 * x;
;   return a + b + c + d;
; }

define dso_local i32 @foo(i32 noundef %x) {
entry:
    %a = mul nsw i32 %x, 1
    %b = mul nsw i32 1, %a
    %c = mul nsw i32 %b, 0
    %d = mul nsw i32 0, %x
    %t0 = add nsw i32 %a, %b
    %t1 = add nsw i32 %t0, %c
    %t2 = add nsw i32 %t1, %d
    ret i32 %t2
}
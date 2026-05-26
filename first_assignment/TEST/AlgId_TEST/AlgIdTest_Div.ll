; int sdiv1(int x) { return x / 1; }
; unsigned udiv1(unsigned x) { return x / 1; }

define dso_local i32 @sdiv1(i32 noundef %x) {
entry:
  %q = sdiv i32 %x, 1
  ret i32 %q
}

define dso_local i32 @udiv1(i32 noundef %x) {
entry:
  %q = udiv i32 %x, 1
  ret i32 %q
}
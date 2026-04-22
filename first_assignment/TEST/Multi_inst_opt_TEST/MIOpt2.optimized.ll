; ModuleID = 'MIOpt2.ll'
source_filename = "MIOpt2.ll"

define dso_local i32 @foo(i32 noundef %0) {
  %2 = add nsw i32 %0, 1
  %3 = sub nsw i32 %2, 1
  ret i32 %0
}

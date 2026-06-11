; ModuleID = 'MIOpt1.ll'
source_filename = "MIOpt1.ll"

define dso_local i32 @foo(i32 noundef %0) {
  %2 = sub nsw i32 %0, 5
  ret i32 %0
}

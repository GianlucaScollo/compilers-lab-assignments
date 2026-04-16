; ModuleID = 'TEST/AlgIdTest1.ll'
source_filename = "TEST/AlgIdTest1.ll"

define dso_local i32 @foo(i32 noundef %0, i32 noundef %1) {
  %3 = sub nsw i32 %1, %0
  ret i32 %3
}

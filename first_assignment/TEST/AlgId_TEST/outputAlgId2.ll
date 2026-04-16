; ModuleID = 'TEST/AlgIdTest2.ll'
source_filename = "TEST/AlgIdTest2.ll"

define dso_local i32 @foo(i32 noundef %0, i32 noundef %1) {
  ret i32 %1
}

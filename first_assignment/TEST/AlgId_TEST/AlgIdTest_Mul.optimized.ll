; ModuleID = 'AlgId_TEST/AlgIdTest_Mul.ll'
source_filename = "AlgId_TEST/AlgIdTest_Mul.ll"

define dso_local i32 @foo(i32 noundef %x) {
entry:
  %t0 = add nsw i32 %x, %x
  ret i32 %t0
}

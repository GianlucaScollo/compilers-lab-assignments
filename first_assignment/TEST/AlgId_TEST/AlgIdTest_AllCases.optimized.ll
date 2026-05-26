; ModuleID = 'AlgId_TEST/AlgIdTest_AllCases.ll'
source_filename = "AlgId_TEST/AlgIdTest_AllCases.ll"

define dso_local i32 @all(i32 noundef %x) {
entry:
  %t0 = add i32 %x, %x
  %t1 = add i32 %t0, %x
  %t2 = add i32 %t1, %x
  %t3 = add i32 %t2, %x
  %t6 = add i32 %t3, %x
  %t7 = add i32 %t6, %x
  %t8 = add i32 %t7, %x
  %t9 = add i32 %t8, %x
  %tA = add i32 %t9, %x
  ret i32 %tA
}

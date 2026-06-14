; ModuleID = 'TEST/Alg_id_TEST/AlgIdTest_AllCases.ll'
source_filename = "TEST/Alg_id_TEST/AlgIdTest_AllCases.ll"

define dso_local i32 @all(i32 noundef %x) {
entry:
  %t0 = add i32 %x, %x
  %t1 = add i32 %t0, %x
  %t2 = add i32 %t1, %x
  %t3 = add i32 %t2, %x
  %t4 = add i32 %t3, %x
  %t5 = add i32 %t4, %x
  %t6 = add i32 %t5, %x
  %t7 = add i32 %t6, %x
  %t8 = add i32 %t7, %x
  ret i32 %t8
}

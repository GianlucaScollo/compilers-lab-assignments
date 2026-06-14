; ModuleID = 'TEST/Alg_id_TEST/AlgIdTest_Shifts.ll'
source_filename = "TEST/Alg_id_TEST/AlgIdTest_Shifts.ll"

define dso_local i32 @foo(i32 noundef %x) {
entry:
  %t0 = add i32 %x, %x
  %t1 = add i32 %t0, %x
  ret i32 %t1
}

; ModuleID = 'TEST/Alg_id_TEST/AlgIdTest_Div.ll'
source_filename = "TEST/Alg_id_TEST/AlgIdTest_Div.ll"

define dso_local i32 @sdiv1(i32 noundef %x) {
entry:
  ret i32 %x
}

define dso_local i32 @udiv1(i32 noundef %x) {
entry:
  ret i32 %x
}

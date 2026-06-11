; ModuleID = 'AlgId_TEST/AlgIdTest_AddSub.ll'
source_filename = "AlgId_TEST/AlgIdTest_AddSub.ll"

define dso_local i32 @foo(i32 noundef %x) {
entry:
  ret i32 %x
}

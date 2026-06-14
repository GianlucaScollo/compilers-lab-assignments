; ModuleID = 'TEST/Multi_inst_opt_TEST/MIOpt2.ll'
source_filename = "TEST/Multi_inst_opt_TEST/MIOpt2.ll"

define dso_local i32 @foo(i32 noundef %0) {
  %2 = add nsw i32 %0, 1
  ret i32 %0
}

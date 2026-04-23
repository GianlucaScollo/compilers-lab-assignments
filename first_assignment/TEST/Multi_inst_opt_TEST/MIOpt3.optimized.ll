; ModuleID = 'MIOpt3.ll'
source_filename = "MIOpt3.ll"

define dso_local i32 @foo(i32 noundef %0) {
  %2 = mul nsw i32 2, %0
  ret i32 %0
}

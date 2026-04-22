; ModuleID = 'MIOpt3.ll'
source_filename = "MIOpt3.ll"

define dso_local i32 @foo(i32 noundef %0) {
  %2 = mul nsw i32 2, %0
  %3 = udiv i32 %2, 2
  ret i32 %0
}

; ModuleID = 'TEST/Str_red_TEST/Foo.ll'
source_filename = "TEST/Str_red_TEST/Foo.ll"

define dso_local i32 @foo(i32 noundef %0, i32 noundef %1) {
  %3 = add nsw i32 %1, 1
  %naf_shl = shl i32 %3, 1
  %4 = shl i32 %0, 1
  %div_ashr = ashr i32 %4, 2
  %5 = mul nsw i32 %naf_shl, %div_ashr
  ret i32 %5
}

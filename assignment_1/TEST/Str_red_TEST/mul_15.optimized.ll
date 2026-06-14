; ModuleID = 'TEST/Str_red_TEST/mul_15.ll'
source_filename = "TEST/Str_red_TEST/mul_15.ll"

define dso_local i32 @moltiplica_per_15(i32 %x) {
entry:
  %naf_shl = shl i32 %x, 4
  %naf_sub = sub i32 %naf_shl, %x
  ret i32 %naf_sub
}

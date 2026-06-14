; ModuleID = 'TEST/Str_red_TEST/mul_-3.ll'
source_filename = "TEST/Str_red_TEST/mul_-3.ll"

define dso_local i32 @moltiplica_per_meno_3(i32 %x) {
entry:
  %naf_shl = shl i32 %x, 2
  %naf_sub = sub i32 %x, %naf_shl
  ret i32 %naf_sub
}

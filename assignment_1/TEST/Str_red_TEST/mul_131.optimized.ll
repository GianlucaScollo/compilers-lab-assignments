; ModuleID = 'TEST/Str_red_TEST/mul_131.ll'
source_filename = "TEST/Str_red_TEST/mul_131.ll"

define dso_local i128 @mul_131(i128 %x) {
entry:
  %naf_shl = shl i128 %x, 2
  %naf_shl1 = shl i128 %x, 7
  %naf_add = add i128 %naf_shl, %naf_shl1
  %naf_sub = sub i128 %naf_add, %x
  ret i128 %naf_sub
}

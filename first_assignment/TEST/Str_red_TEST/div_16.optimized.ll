; ModuleID = 'TEST/Str_red_TEST/div_16.ll'
source_filename = "TEST/Str_red_TEST/div_16.ll"

define dso_local i128 @divisione_16(i128 %x) {
entry:
  %div_ashr = ashr i128 %x, 4
  ret i128 %div_ashr
}

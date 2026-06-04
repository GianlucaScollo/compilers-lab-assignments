; ModuleID = 'TEST/Str_red_TEST/div_20.ll'
source_filename = "TEST/Str_red_TEST/div_20.ll"

define dso_local i128 @divisione_20(i128 %x) {
entry:
  %x_allargata_segno = sext i128 %x to i256
  %moltiplicazione_magica = mul i256 %x_allargata_segno, 136112946768375385385349842972707284583
  %0 = lshr i256 %moltiplicazione_magica, 128
  %parte_alta_32bit = trunc i256 %0 to i128
  %shift_aritmetico_finale = ashr i128 %parte_alta_32bit, 3
  %estrai_bit_segno = lshr i128 %x, 127
  %risultato_sdiv_corretto = add i128 %shift_aritmetico_finale, %estrai_bit_segno
  ret i128 %risultato_sdiv_corretto
}

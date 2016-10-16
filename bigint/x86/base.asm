 segment .text

; bigint_word_t _bigint_ww_mul_impl(bigint_word_t a, bigint_word_t b, bigint_word_t *res_hi);
; a      = ecx
; b      = edx
; res_hi = [esp + 4]

 align 8
 global @_bigint_ww_mul_impl@12

@_bigint_ww_mul_impl@12:
 mov     eax, edx
 mul     ecx
 mov     ecx, [esp+4]
 mov     [ecx], edx
 ret     4

; bigint_word_t _bigint_dw_ndiv_impl(bigint_word_t a_lo, bigint_word_t a_hi, bigint_word_t b, bigint_word_t *r);
; a_lo  = ecx
; a_hi  = edx
; b     = [esp + 4]
; r     = [esp + 8]

 align 8
 global @_bigint_dw_ndiv_impl@16

@_bigint_dw_ndiv_impl@16:
 mov     eax, ecx
 mov     ecx, [esp+4]
 div     ecx
 mov     ecx, [esp+8]
 mov     [ecx], edx
 ret     8

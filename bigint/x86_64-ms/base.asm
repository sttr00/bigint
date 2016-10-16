 segment .text

; bigint_word_t _bigint_ww_mul_impl(bigint_word_t a, bigint_word_t b, bigint_word_t *res_hi);
; a      = rcx
; b      = rdx
; res_hi = r8

 align 8
 global _bigint_ww_mul_impl

_bigint_ww_mul_impl:
 mov     rax, rdx
 mul     rcx
 mov     [r8], rdx
 ret

; bigint_word_t _bigint_dw_ndiv_impl(bigint_word_t a_lo, bigint_word_t a_hi, bigint_word_t b, bigint_word_t *r);
; a_lo  = rcx
; a_hi  = rdx
; b     = r8
; r     = r9

 align 8
 global _bigint_dw_ndiv_impl

_bigint_dw_ndiv_impl:
 mov     rax, rcx
 div     r8
 mov     [r9], rdx
 ret

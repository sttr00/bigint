/*
 bigint_word_t _bigint_dw_ndiv_impl(bigint_word_t a_lo, bigint_word_t a_hi, bigint_word_t b, bigint_word_t *r);
  a_lo = r0
  a_hi = r1
  b    = r2
  r    = r3
*/

.global _bigint_dw_ndiv_impl
.hidden _bigint_dw_ndiv_impl

_bigint_dw_ndiv_impl:
 stmfd   sp!, {r4-r7}

 mov     r7, #0                  /* quotient */
 mov     r6, #32                 /* loop counter */

loop:
 mov     r5, r1, lsr #31         /* flag */
 mov     r1, r1, lsl #1
 orr     r1, r1, r0, lsr #31
 mov     r0, r0, lsl #1

 mov     r7, r7, lsl #1
 subs    r4, r1, r2
 sbcs    r5, r5, #0
 movcs   r1, r4
 adc     r7, r7, #0

 subs    r6, r6, #1
 bne     loop

 mov     r0, r7
 str     r1, [r3]
 ldmfd   sp!, {r4-r7}
 mov     pc, lr

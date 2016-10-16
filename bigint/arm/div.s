/*
 bigint_word_t _bigint_td_ndiv_impl(const bigint_word_t *u, bigint_word_t v, bigint_word_t v2)
  u  = r0
  v  = r1
  v2 = r2
*/

.global _bigint_td_ndiv_impl
.hidden _bigint_td_ndiv_impl

_bigint_td_ndiv_impl:
 stmfd   sp!, {r3-r8}
 ldmia   r0, {r3-r5}
 cmp     r5, r1
 bhs     overflow

 mov     r0, #0                  /* quotient */
 mov     r7, #32                 /* loop counter */

loop:
 mov     r8, r5, lsr #31         /* flag */
 mov     r5, r5, lsl #1
 orr     r5, r5, r4, lsr #31
 mov     r4, r4, lsl #1

 mov     r0, r0, lsl #1
 subs    r6, r5, r1
 sbcs    r8, r8, #0
 movcs   r5, r6
 adc     r0, r0, #0

 subs    r7, r7, #1
 bne     loop

 /* r5 = remainder */
 umull   r7, r4, r2, r0
repeat:
 subs    r7, r7, r3
 sbcs    r4, r4, r5
 bhi     decrease

 ldmfd   sp!, {r3-r8}
 mov     pc, lr
decrease:
 subs    r7, r7, r2
 sbc     r4, r4, #0
 sub     r0, r0, #1
 adds    r5, r5, r1
 bcc     repeat

 ldmfd   sp!, {r3-r8}
 mov     pc, lr

overflow:
 mov     r0, #-1
 adds    r8, r4, r5
 bcs     fin
 umull   r7, r4, r2, r0
 subs    r7, r7, r3
 sbcs    r4, r4, r5
 bhi     decrease

fin:
 ldmfd   sp!, {r3-r8}
 mov     pc, lr

/*
 bigint_word_t _bigint_dw_ndiv_impl(bigint_word_t a_lo, bigint_word_t a_hi, bigint_word_t b, bigint_word_t *r);
  a_lo = x0
  a_hi = x1
  b    = x2
  r    = x3
 */

.global _bigint_dw_ndiv_impl
.hidden _bigint_dw_ndiv_impl

_bigint_dw_ndiv_impl:
 mov     x13, 0xFFFFFFFF
 lsr     x5, x2, #32  /* x5 = divisor halfword */
 udiv    x4, x1, x5
 cmp     x4, x13
 csel    x4, x13, x4, hi
 lsl     x4, x4, #32  /* x4 = q^ */
 mov     x10, #0x100000000
 mul     x6, x4, x2
 umulh   x7, x4, x2   /* <x7, x6> = b*q^ */

loop1:
 subs    x8, x0, x6
 sbcs    x9, x1, x7
 b.cs    next
 sub     x4, x4, x10
 lsr     x11, x2, #32
 subs    x6, x6, x2, lsl #32
 sbc     x7, x7, x11
 b       loop1

next:
 lsl     x11, x9, #32
 orr     x11, x11, x8, lsr #32
 udiv    x10, x11, x5  /* x10 = lower half of the quotient */
 cmp     x10, x13
 csel    x10, x13, x10, hi
 mul     x6, x10, x2
 umulh   x7, x10, x2

loop2:
 subs    x1, x8, x6
 sbcs    xzr, x9, x7
 b.cs    fin
 sub     x10, x10, #1
 subs    x6, x6, x2
 sbc     x7, x7, xzr
 b       loop2

fin:
 str     x1, [x3]
 orr     x0, x10, x4
 ret

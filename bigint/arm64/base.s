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
 lsr     x5, x2, #32  /* x5 = b1 */
 mov     w7, w2       /* x7 = b0 */
 udiv    x4, x1, x5   /* x4 = q1 */
 mul     x6, x4, x5
 sub     x6, x1, x6   /* x6 = r */
 mul     x8, x4, x7
 lsr     x9, x0, #32
 orr     x9, x9, x6, lsl #32 /* x9 = r*B + a1 */
 cmp     x9, x8
 b.hs    skip1
 sub     x4, x4, #1
 adds    x9, x9, x2
 b.cs    skip1
 cmp     x9, x8
 b.hs    skip1
 sub     x4, x4, #1
 add     x9, x9, x2
skip1:
 sub     x9, x9, x8
 /* second halfword */
 udiv    x10, x9, x5  /* x10 = q0 */
 mul     x6, x10, x5
 sub     x6, x9, x6
 mul     x8, x10, x7
 mov     w11, w0
 orr     x11, x11, x6, lsl #32
 cmp     x11, x8
 b.hs    skip2
 sub     x10, x10, #1
 adds    x11, x11, x2
 b.cs    skip2
 cmp     x11, x8
 b.hs    skip2
 sub     x10, x10, #1
 add     x11, x11, x2
skip2:
 sub     x11, x11, x8
 lsl     x0, x4, #32
 orr     x0, x0, x10
 str     x11, [x3]
 ret

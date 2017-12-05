/*
 bigint_word_t _bigint_td_ndiv_impl(const bigint_word_t *u, bigint_word_t v, bigint_word_t v2);
  u  = x0
  v  = x1
  v2 = x2
*/

.global _bigint_td_ndiv_impl
.hidden _bigint_td_ndiv_impl

_bigint_td_ndiv_impl:
 ldp     x3, x4, [x0]
 ldr     x5, [x0, #16]
 cmp     x5, x1
 b.hs    overflow

 lsr     x6, x1, #32  /* x6 = b1 */
 mov     w7, w1       /* x7 = b0 */
 udiv    x8, x5, x6   /* x8 = q1 */
 mul     x9, x8, x6
 sub     x9, x5, x9   /* x9 = r */
 mul     x10, x8, x7  /* x10 = q1*b0 */
 lsr     x11, x4, #32
 orr     x11, x11, x9, lsl #32 /* x11 = r*B + a1 */
 cmp     x11, x10
 b.hs    skip1
 sub     x8, x8, #1
 adds    x11, x11, x1
 b.cs    skip1
 cmp     x11, x10
 b.hs    skip1
 sub     x8, x8, #1
 add     x11, x11, x1
skip1:
 sub     x11, x11, x10
 /* second halfword */
 udiv    x0, x11, x6  /* x0 = q0 */
 mul     x12, x0, x6
 sub     x5, x11, x12 /* x5 = r */
 mul     x10, x0, x7  /* x10 = q0*b0 */
 mov     w11, w4
 orr     x11, x11, x5, lsl #32 /* x11 = r*B + a0 */
 cmp     x11, x10
 b.hs    skip2
 sub     x0, x0, #1
 adds    x11, x11, x1
 b.cs    skip2
 cmp     x11, x10
 b.hs    skip2
 sub     x0, x0, #1
 add     x11, x11, x1
skip2:
 sub     x5, x11, x10 /* x5 = r^ */
 orr     x0, x0, x8, lsl #32 /* x0 = q^ */

 mul     x7, x2, x0
 umulh   x4, x2, x0
repeat:
 subs    x7, x7, x3
 sbcs    x4, x4, x5
 b.hi    decrease
 ret

decrease:
 subs    x7, x7, x2
 sbc     x4, x4, xzr
 sub     x0, x0, #1
 adds    x5, x5, x1
 b.cc    repeat
 ret

overflow:
 mov     x0, #-1
 adds    x8, x4, x5
 b.cs    fin
 mul     x7, x2, x0
 umulh   x4, x2, x0
 subs    x7, x7, x3
 sbcs    x4, x4, x5
 b.hi    decrease

fin:
 ret

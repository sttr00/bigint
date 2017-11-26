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
 lsr     x6, x1, #32   /* x6 = divisor halfword */
 cmp     x6, x5, lsr #32
 b.ne    div1
 mov     x0, #0xFFFFFFFF00000000
 b       div2
div1:
 udiv    x0, x5, x6
 lsl     x0, x0, #32   /* x0 = q^ */
div2:
 mov     x10, #0x100000000
 mul     x7, x0, x1
 umulh   x12, x0, x1   /* <x12, x7> = b*q^ */

loop1:
 subs    x8, x4, x7
 sbcs    x9, x5, x12
 b.cs    next
 sub     x0, x0, x10
 lsr     x11, x1, #32
 subs    x7, x7, x1, lsl #32
 sbc     x12, x12, x11
 b       loop1

next:
 lsl     x11, x9, #32
 orr     x11, x11, x8, lsr #32
 udiv    x10, x11, x6  /* x10 = lower half of the quotient */
 mul     x7, x10, x1
 umulh   x12, x10, x1

loop2:
 subs    x5, x8, x7
 sbcs    xzr, x9, x12
 b.cs    div_done
 sub     x10, x10, #1
 subs    x7, x7, x1
 sbc     x12, x12, xzr
 b       loop2

div_done:
 /* x5 = remainder, x0 = quotient */
 orr     x0, x0, x10

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

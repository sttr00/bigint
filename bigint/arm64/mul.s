/*
 void _bigint_mulw(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
  res  = x0
  a    = x1
  w    = x2
  size = w3
 */

.global _bigint_mulw
.hidden _bigint_mulw

_bigint_mulw:
 mov     x4, xzr

loop1:
 ldr     x5, [x1], #8
 mul     x6, x2, x5
 umulh   x7, x2, x5
 adds    x6, x6, x4
 str     x6, [x0], #8
 adc     x4, x7, xzr
 subs    w3, w3, #1
 b.ne    loop1

 str     x4, [x0]
 ret

/*
 void _bigint_muladdw(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
  res  = x0
  a    = x1
  w    = x2
  size = w3
 */

.global _bigint_muladdw
.hidden _bigint_muladdw

_bigint_muladdw:
 mov     x4, xzr

loop2:
 ldr     x5, [x1], #8
 mul     x6, x2, x5
 umulh   x7, x2, x5 
 adds    x6, x6, x4
 adc     x4, x7, xzr
 ldr     x5, [x0]
 adds    x6, x6, x5
 adc     x4, x4, xzr
 str     x6, [x0], #8
 subs    w3, w3, #1
 b.ne    loop2

 str     x4, [x0]
 ret

/*
 bigint_word_t _bigint_mulsubw(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
  res  = x0
  a    = x1
  w    = x2
  size = w3
 */

.global _bigint_mulsubw
.hidden _bigint_mulsubw

_bigint_mulsubw:
 mov     x4, xzr

loop3:
 ldr     x5, [x1], #8
 mul     x6, x2, x5
 umulh   x7, x2, x5
 adds    x6, x6, x4
 adc     x4, x7, xzr
 ldr     x5, [x0]
 subs    x6, x5, x6
 sbc     x7, xzr, xzr
 str     x6, [x0], #8
 sub     x4, x4, x7
 subs    w3, w3, #1
 b.ne    loop3

 ldr     x5, [x0]
 subs    x5, x5, x4
 str     x5, [x0]
 csinc   x0, xzr, xzr, cs

 ret

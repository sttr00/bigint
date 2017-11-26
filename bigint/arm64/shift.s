/*
 bigint_word_t _bigint_shl(bigint_word_t *res, const bigint_word_t *a, int bits, int size);
  res  = x0
  a    = x1
  bits = w2
  size = w3
 */

.global _bigint_shl
.hidden _bigint_shl

_bigint_shl:
 sub     w4, w2, #64
 neg     w4, w4
 mov     x5, xzr

loop1:
 ldr     x6, [x1], #8
 lslv    x7, x6, x2
 orr     x7, x7, x5
 str     x7, [x0], #8
 subs    w3, w3, #1
 lsrv    x5, x6, x4
 b.ne    loop1

 mov     x0, x5
 ret

/*
 bigint_word_t _bigint_shr(bigint_word_t *res, const bigint_word_t *a, int bits, int size);
  res  = x0
  a    = x1
  bits = w2
  size = w3
 */

.global _bigint_shr
.hidden _bigint_shr

_bigint_shr:
 sub     w4, w2, #64
 neg     w4, w4
 mov     x5, xzr
 sxtw    x3, w3
 add     x0, x0, x3, lsl #3
 add     x1, x1, x3, lsl #3

loop2:
 ldr     x6, [x1, #-8]!
 lsrv    x7, x6, x2
 orr     x7, x7, x5
 str     x7, [x0, #-8]!
 subs    w3, w3, #1
 lslv    x5, x6, x4
 b.ne    loop2

 lsrv    x0, x5, x4
 ret

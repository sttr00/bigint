/*
 bigint_word_t _bigint_add(bigint_word_t *res, const bigint_word_t *a, const bigint_word_t *b, int size);
  res  = x0
  a    = x1
  b    = x2
  size = w3
 */

.global _bigint_add
.hidden _bigint_add

_bigint_add:
 mov     x4, xzr

loop1:
 adds    x6, x4, #-1
 ldr     x5, [x1], #8
 ldr     x6, [x2], #8
 adcs    x5, x5, x6
 str     x5, [x0], #8
 adc     x4, xzr, xzr
 subs    w3, w3, #1
 b.ne    loop1

 mov     x0, x4
 ret

/*
 bigint_word_t _bigint_addw(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
  res  = x0
  a    = x1
  w    = x2
  size = w3
 */

.global _bigint_addw
.hidden _bigint_addw

_bigint_addw:
 ldr     x4, [x1], #8
 adds    x4, x4, x2
 str     x4, [x0], #8
 b.cc    copy1

 subs    w3, w3, #1
 b.eq    fin1
loop2:
 ldr     x4, [x1], #8
 adds    x4, x4, #1
 str     x4, [x0], #8
 b.cc    copy1
 subs    w3, w3, #1
 b.ne    loop2

fin1:
 mov     x0, #1
 ret

copy1:
 subs    w3, w3, #1
 b.eq    fin2
loop3:
 ldr     x4, [x1], #8
 subs    w3, w3, #1
 str     x4, [x0], #8
 b.ne    loop3

fin2:
 mov     x0, xzr
 ret

/*
 bigint_word_t _bigint_sub(bigint_word_t *res, const bigint_word_t *a, const bigint_word_t *b, int size);
  res  = x0
  a    = x1
  b    = x2
  size = w3
 */

.global _bigint_sub
.hidden _bigint_sub

_bigint_sub:
 mov     x4, #1

loop4:
 adds    x6, x4, #-1
 ldr     x5, [x1], #8
 ldr     x6, [x2], #8
 sbcs    x5, x5, x6
 str     x5, [x0], #8
 adc     x4, xzr, xzr
 subs    w3, w3, #1
 b.ne    loop4

 eor     x0, x4, #1
 ret

/*
 bigint_word_t _bigint_subw(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
  res  = x0
  a    = x1
  w    = x2
  size = w3
 */

.global _bigint_subw
.hidden _bigint_subw

_bigint_subw:
 ldr     x4, [x1], #8
 subs    x4, x4, x2
 str     x4, [x0], #8
 b.cs    copy2

 subs    w3, w3, #1
 b.eq    fin3
loop5:
 ldr     x4, [x1], #8
 subs    x4, x4, #1
 str     x4, [x0], #8
 b.cs    copy2
 subs    w3, w3, #1
 b.ne    loop5

fin3:
 mov     x0, #1
 ret

copy2:
 subs    w3, w3, #1
 b.eq    fin4
loop6:
 ldr     x4, [x1], #8
 subs    w3, w3, #1
 str     x4, [x0], #8
 b.ne    loop6

fin4:
 mov     x0, xzr
 ret

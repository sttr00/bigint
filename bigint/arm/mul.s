/*
 void _bigint_mulw(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
  res  = r0
  a    = r1
  w    = r2
  size = r3
 */

.global _bigint_mulw
.hidden _bigint_mulw

_bigint_mulw:
 stmfd   sp!, {r4-r6}
 mov     r4, #0
 mov     r6, #0

loop1:
 ldr     r5, [r1], #4
 umlal   r4, r6, r2, r5
 str     r4, [r0], #4
 subs    r3, r3, #1
 mov     r4, r6
 mov     r6, #0
 bne     loop1

 str     r4, [r0]
 ldmfd   sp!, {r4-r6}
 mov     pc, lr

/*
 void _bigint_muladdw(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
  res  = r0
  a    = r1
  w    = r2
  size = r3
 */

.global _bigint_muladdw
.hidden _bigint_muladdw

_bigint_muladdw:
 stmfd   sp!, {r4-r6}
 mov     r4, #0

loop2:
 ldr     r6, [r0]
 ldr     r5, [r1], #4
 umaal   r4, r6, r2, r5
 str     r4, [r0], #4
 subs    r3, r3, #1
 mov     r4, r6
 bne     loop2

 str     r4, [r0]
 ldmfd   sp!, {r4-r6}
 mov     pc, lr

/*
 bigint_word_t _bigint_mulsubw(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
  res  = r0
  a    = r1
  w    = r2
  size = r3
 */

.global _bigint_mulsubw
.hidden _bigint_mulsubw

_bigint_mulsubw:
 stmfd   sp!, {r4-r7}
 mov     r4, #0
 mov     r7, #0

loop3:
 ldr     r5, [r1], #4
 ldr     r6, [r0]
 umlal   r4, r7, r2, r5
 subs    r6, r6, r4
 addcc   r7, r7, #1
 str     r6, [r0], #4
 subs    r3, r3, #1
 mov     r4, r7
 mov     r7, #0
 bne     loop3

 ldr     r5, [r0]
 subs    r5, r5, r4
 str     r5, [r0]
 movcc   r0, #1
 movcs   r0, #0

 ldmfd   sp!, {r4-r7}
 mov     pc, lr

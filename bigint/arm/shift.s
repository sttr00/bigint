/*
 bigint_word_t _bigint_shl(bigint_word_t *res, const bigint_word_t *a, int bits, int size);
  res  = r0
  a    = r1
  bits = r2
  size = r3
 */

.global _bigint_shl
.hidden _bigint_shl

_bigint_shl:
 stmfd   sp!, {r4-r6}
 rsb     r4, r2, #32
 mov     r5, #0

loop1:
 ldr     r6, [r1], #4
 orr     r5, r5, r6, lsl r2
 str     r5, [r0], #4
 subs    r3, r3, #1
 mov     r5, r6, lsr r4
 bne     loop1

 mov     r0, r5
 ldmfd   sp!, {r4-r6}
 mov     pc, lr

/*
 bigint_word_t _bigint_shr(bigint_word_t *res, const bigint_word_t *a, int bits, int size);
  res  = r0
  a    = r1
  bits = r2
  size = r3
 */

.global _bigint_shr
.hidden _bigint_shr

_bigint_shr:
 stmfd   sp!, {r4-r6}
 rsb     r4, r2, #32
 mov     r5, #0
 add     r0, r0, r3, lsl #2
 add     r1, r1, r3, lsl #2

loop2:
 ldr     r6, [r1, #-4]!
 orr     r5, r5, r6, lsr r2
 str     r5, [r0, #-4]!
 subs    r3, r3, #1
 mov     r5, r6, lsl r4
 bne     loop2

 mov     r0, r5
 ldmfd   sp!, {r4-r6}
 mov     pc, lr

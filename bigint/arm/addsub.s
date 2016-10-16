/*
 bigint_word_t _bigint_add(bigint_word_t *res, const bigint_word_t *a, const bigint_word_t *b, int size);
  res  = r0
  a    = r1
  b    = r2
  size = r3
 */

.global _bigint_add
.hidden _bigint_add

_bigint_add:
 stmfd   sp!, {r4-r7}
 mov     r4, #0
 mov     r7, #0

loop1:
 adds    r6, r4, #-1   @ cmp r4, #1
 ldr     r5, [r1], #4
 ldr     r6, [r2], #4
 adcs    r5, r5, r6
 str     r5, [r0], #4
 adc     r4, r7, r7
 subs    r3, r3, #1
 bne     loop1

 mov     r0, r4
 ldmfd   sp!, {r4-r7}
 mov     pc, lr

/*
 bigint_word_t _bigint_addw(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
  res  = r0
  a    = r1
  w    = r2
  size = r3
 */

.global _bigint_addw
.hidden _bigint_addw

_bigint_addw:
 str     r4, [sp, #-4]!

 ldr     r4, [r1], #4
 adds    r4, r4, r2
 str     r4, [r0], #4
 bcc     copy1

 subs    r3, r3, #1
 beq     fin1
loop2:
 ldr     r4, [r1], #4
 adds    r4, r4, #1
 str     r4, [r0], #4
 bcc     copy1
 subs    r3, r3, #1
 bne     loop2

fin1:
 mov     r0, #1
 ldmfd   sp!, {r4}
 mov     pc, lr

copy1:
 subs    r3, r3, #1
 beq     fin2
loop3:
 ldr     r4, [r1], #4
 subs    r3, r3, #1
 str     r4, [r0], #4
 bne     loop3 

fin2:
 mov     r0, #0
 ldr     r4, [sp], #4
 mov     pc, lr

/*
 bigint_word_t _bigint_sub(bigint_word_t *res, const bigint_word_t *a, const bigint_word_t *b, int size);
  res  = r0
  a    = r1
  b    = r2
  size = r3
 */

.global _bigint_sub
.hidden _bigint_sub

_bigint_sub:
 stmfd   sp!, {r4-r7}
 mov     r4, #1
 mov     r7, #0

loop4:
 adds    r6, r4, #-1
 ldr     r5, [r1], #4
 ldr     r6, [r2], #4
 sbcs    r5, r5, r6
 str     r5, [r0], #4
 adc     r4, r7, r7
 subs    r3, r3, #1
 bne     loop4

 eor     r0, r4, #1
 ldmfd   sp!, {r4-r7}
 mov     pc, lr

/*
 bigint_word_t _bigint_subw(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
  res  = r0
  a    = r1
  w    = r2
  size = r3
 */

.global _bigint_subw
.hidden _bigint_subw

_bigint_subw:
 str     r4, [sp, #-4]!

 ldr     r4, [r1], #4
 subs    r4, r4, r2
 str     r4, [r0], #4
 bcs     copy2

 subs    r3, r3, #1
 beq     fin3
loop5:
 ldr     r4, [r1], #4
 subs    r4, r4, #1
 str     r4, [r0], #4
 bcs     copy2
 subs    r3, r3, #1
 bne     loop5

fin3:
 mov     r0, #1
 ldmfd   sp!, {r4}
 mov     pc, lr

copy2:
 subs    r3, r3, #1
 beq     fin4
loop6:
 ldr     r4, [r1], #4
 subs    r3, r3, #1
 str     r4, [r0], #4
 bne     loop6

fin4:
 mov     r0, #0
 ldr     r4, [sp], #4
 mov     pc, lr

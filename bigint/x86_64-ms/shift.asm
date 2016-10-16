 segment .text

; bigint_word_t _bigint_shl(bigint_word_t *res, const bigint_word_t *a, int bits, int size);
; res  = rcx
; a    = rdx
; bits = r8
; size = r9

 align 16
 global _bigint_shl

_bigint_shl:
 mov     r10, rcx
 xor     r11, r11
 mov     rcx, r8

 align 16

loop1:
 mov     rax, [rdx]
 mov     r8, rax
 shld    rax, r11, cl
 mov     r11, r8
 mov     [r10], rax
 lea     rdx, [rdx+8]
 lea     r10, [r10+8]
 dec     r9
 jnz     loop1

 xor     rax, rax
 shld    rax, r11, cl
 ret

; bigint_word_t _bigint_shr(bigint_word_t *res, const bigint_word_t *a, int bits, int size);
; res  = rcx
; a    = rdx
; bits = r8
; size = r9

 align 16
 global _bigint_shr

_bigint_shr:
 mov     r10, rcx
 xor     r11, r11
 mov     rcx, r8

 dec     r9
 lea     r10, [r10+r9*8]
 lea     rdx, [rdx+r9*8]
 inc     r9

 align 16

loop2:
 mov     rax, [rdx]
 mov     r8, rax
 shrd    rax, r11, cl
 mov     r11, r8
 mov     [r10], rax
 lea     rdx, [rdx-8]
 lea     r10, [r10-8]
 dec     r9
 jnz     loop2

 xor     rax, rax
 shrd    rax, r11, cl
 ret

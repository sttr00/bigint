 segment .text

; void _bigint_mulw_mulx(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = rcx
; a    = rdx
; w    = r8
; size = r9

 align 16
 global _bigint_mulw_mulx

_bigint_mulw_mulx:
 xor     r10, r10
 xchg    rdx, r8

 align 16

loop1:
 mulx    r11, rax, [r8]
 adc     rax, r10
 mov     [rcx], rax
 lea     r8, [r8+8]
 lea     rcx, [rcx+8]
 mov     r10, r11
 dec     r9
 jnz     loop1

 mov     rax, r10
 adc     rax, r9
 mov     [rcx], rax
 ret

; void _bigint_muladdw_mulx(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = rcx
; a    = rdx
; w    = r8
; size = r9

 align 16
 global _bigint_muladdw_mulx

_bigint_muladdw_mulx:
 xor     r10, r10
 xchg    rdx, r8

 align 16

loop2:
 mulx    r11, rax, [r8]
 adc     rax, r10
 adc     r11, 0
 add     [rcx], rax
 lea     r8, [r8+8]
 lea     rcx, [rcx+8]
 mov     r10, r11
 dec     r9
 jnz     loop2

 mov     rax, r10
 adc     rax, r9
 mov     [rcx], rax
 ret

; bigint_word_t _bigint_mulsubw_mulx(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = rcx
; a    = rdx
; w    = r8
; size = r9

 align 16
 global _bigint_mulsubw_mulx

_bigint_mulsubw_mulx:
 xor     r10, r10
 xchg    rdx, r8

 align 16

loop3:
 mulx    r11, rax, [r8]
 adc     rax, r10
 adc     r11, 0
 sub     [rcx], rax
 lea     r8, [r8+8]
 lea     rcx, [rcx+8]
 mov     r10, r11
 dec     r9
 jnz     loop3

 sbb     [rcx], r10
 mov     rax, r9
 adc     rax, rax
 ret

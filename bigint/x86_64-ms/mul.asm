 segment .text

; void _bigint_mulw(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = rcx
; a    = rdx
; w    = r8
; size = r9

 align 16
 global _bigint_mulw

_bigint_mulw:
 xor     r10, r10
 mov     r11, rdx

 align 16

loop1:
 mov     rax, [r11]
 mul     r8
 add     rax, r10
 adc     rdx, 0
 mov     [rcx], rax
 mov     r10, rdx
 lea     rcx, [rcx+8]
 lea     r11, [r11+8]
 dec     r9
 jnz     loop1

 mov     [rcx], r10
 ret

; void _bigint_muladdw(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = rcx
; a    = rdx
; w    = r8
; size = r9

 align 16
 global _bigint_muladdw

_bigint_muladdw:
 xor     r10, r10
 mov     r11, rdx

 align 16

loop2:
 mov     rax, [r11]
 mul     r8 
 add     rax, r10
 adc     rdx, 0
 add     [rcx], rax
 adc     rdx, 0
 lea     rcx, [rcx+8]
 lea     r11, [r11+8]
 mov     r10, rdx
 dec     r9
 jnz     loop2

 mov     [rcx], r10
 ret

; bigint_word_t _bigint_mulsubw(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = rcx
; a    = rdx
; w    = r8
; size = r9

 align 16
 global _bigint_mulsubw

_bigint_mulsubw:
 xor    r10, r10
 mov    r11, rdx

 align 16

loop3:
 mov     rax, [r11]
 mul     r8
 add     rax, r10
 adc     rdx, 0
 sub     [rcx], rax
 adc     rdx, 0
 lea     rcx, [rcx+8]
 lea     r11, [r11+8]
 mov     r10, rdx
 dec     r9
 jnz     loop3

 sub     [rcx], r10
 mov     rax, r9
 adc     rax, rax
 ret

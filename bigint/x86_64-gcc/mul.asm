 segment .text

; void _bigint_mulw(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = rdi
; a    = rsi
; w    = rdx
; size = rcx

 align 16
 global _bigint_mulw

_bigint_mulw:
 xor     r10, r10
 mov     r8, rdx

 align 16

loop1:
 mov     rax, [rsi]
 mul     r8
 add     rax, r10
 adc     rdx, 0
 mov     [rdi], rax
 mov     r10, rdx
 lea     rsi, [rsi+8]
 lea     rdi, [rdi+8]
 dec     rcx
 jnz     loop1

 mov     [rdi], r10
 ret

; void _bigint_muladdw(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = rdi
; a    = rsi
; w    = rdx
; size = rcx

 align 16
 global _bigint_muladdw

_bigint_muladdw:
 xor     r10, r10
 mov     r8, rdx

 align 16

loop2:
 mov     rax, [rsi]
 mul     r8 
 add     rax, r10
 adc     rdx, 0
 add     [rdi], rax
 adc     rdx, 0
 lea     rsi, [rsi+8]
 lea     rdi, [rdi+8]
 mov     r10, rdx
 dec     rcx
 jnz     loop2

 mov     [rdi], r10
 ret

; bigint_word_t _bigint_mulsubw(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = rdi
; a    = rsi
; w    = rdx
; size = rcx

 align 16
 global _bigint_mulsubw

_bigint_mulsubw:
 xor    r10, r10
 mov    r8, rdx

 align 16

loop3:
 mov     rax, [rsi]
 mul     r8
 add     rax, r10
 adc     rdx, 0
 sub     [rdi], rax
 adc     rdx, 0
 lea     rsi, [rsi+8]
 lea     rdi, [rdi+8]
 mov     r10, rdx
 dec     rcx
 jnz     loop3

 sub     [rdi], r10
 mov     rax, rcx
 adc     rax, rax
 ret

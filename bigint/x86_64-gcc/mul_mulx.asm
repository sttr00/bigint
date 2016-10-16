 segment .text

; void _bigint_mulw_mulx(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = rdi
; a    = rsi
; w    = rdx
; size = rcx

 align 16
 global _bigint_mulw_mulx

_bigint_mulw_mulx:
 xor     r10, r10

 align 16

loop1:
 mulx    r11, rax, [rsi]
 adc     rax, r10
 mov     [rdi], rax
 lea     rsi, [rsi+8]
 lea     rdi, [rdi+8]
 mov     r10, r11
 dec     rcx
 jnz     loop1

 mov     rax, r10
 adc     rax, rcx
 mov     [rdi], rax
 ret

; void _bigint_muladdw_mulx(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = rdi
; a    = rsi
; w    = rdx
; size = rcx

 align 16
 global _bigint_muladdw_mulx

_bigint_muladdw_mulx:
 xor     r10, r10

 align 16

loop2:
 mulx    r11, rax, [rsi]
 adc     rax, r10
 adc     r11, 0
 add     [rdi], rax
 lea     rsi, [rsi+8]
 lea     rdi, [rdi+8]
 mov     r10, r11
 dec     rcx
 jnz     loop2

 mov     rax, r10
 adc     rax, rcx
 mov     [rdi], rax
 ret

; bigint_word_t _bigint_mulsubw_mulx(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = rdi
; a    = rsi
; w    = rdx
; size = rcx

 align 16
 global _bigint_mulsubw_mulx

_bigint_mulsubw_mulx:
 xor     r10, r10

 align 16

loop3:
 mulx    r11, rax, [rsi]
 adc     rax, r10
 adc     r11, 0
 sub     [rdi], rax
 lea     rsi, [rsi+8]
 lea     rdi, [rdi+8]
 mov     r10, r11
 dec     rcx
 jnz     loop3

 sbb     [rdi], r10
 mov     rax, rcx
 adc     rax, rax
 ret

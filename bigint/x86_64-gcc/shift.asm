 segment .text

; bigint_word_t _bigint_shl(bigint_word_t *res, const bigint_word_t *a, int bits, int size);
; res  = rdi
; a    = rsi
; bits = rdx
; size = rcx

 align 16
 global _bigint_shl

_bigint_shl:
 mov     r9, rcx
 xor     r11, r11
 mov     rcx, rdx

 align 16

loop1:
 mov     rax, [rsi]
 mov     r8, rax
 shld    rax, r11, cl
 mov     r11, r8
 mov     [rdi], rax
 lea     rsi, [rsi+8]
 lea     rdi, [rdi+8]
 dec     r9
 jnz     loop1

 xor     rax, rax
 shld    rax, r11, cl
 ret

; bigint_word_t _bigint_shr(bigint_word_t *res, const bigint_word_t *a, int bits, int size);
; res  = rdi
; a    = rsi
; bits = rdx
; size = rcx

 align 16
 global _bigint_shr

_bigint_shr:
 mov     r9, rcx
 xor     r11, r11
 mov     rcx, rdx

 dec     r9
 lea     rsi, [rsi+r9*8]
 lea     rdi, [rdi+r9*8]
 inc     r9

 align 16

loop2:
 mov     rax, [rsi]
 mov     r8, rax
 shrd    rax, r11, cl
 mov     r11, r8
 mov     [rdi], rax
 lea     rsi, [rsi-8]
 lea     rdi, [rdi-8]
 dec     r9
 jnz     loop2

 xor     rax, rax
 shrd    rax, r11, cl
 ret

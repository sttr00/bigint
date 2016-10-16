 segment .text

; bigint_word_t _bigint_add(bigint_word_t *res, const bigint_word_t *a, const bigint_word_t *b, int size);
; res  = rdi
; a    = rsi
; b    = rdx
; size = rcx

 align 16
 global _bigint_add

_bigint_add:
 lea     rdi, [rdi+rcx*8]
 lea     rsi, [rsi+rcx*8]
 lea     rdx, [rdx+rcx*8]
 neg     rcx
 xor     rax, rax

 align 16

loop1:
 mov     rax, [rsi+rcx*8]
 adc     rax, [rdx+rcx*8]
 mov     [rdi+rcx*8], rax
 inc     rcx
 jnz     loop1

 mov     rax, rcx
 adc     rax, rax
 ret

; bigint_word_t _bigint_addw(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = rdi
; a    = rsi
; w    = rdx
; size = rcx

 align 16
 global _bigint_addw

_bigint_addw:
 lea     rdi, [rdi+rcx*8]
 lea     rsi, [rsi+rcx*8]
 neg     rcx
 xor     rax, rax

 align 16

loop2:
 mov     rax, [rsi+rcx*8]
 adc     rax, rdx
 mov     [rdi+rcx*8], rax
 jnc     break2
 mov     rdx, 0
 inc     rcx
 jnz     loop2

fin2:
 mov     rax, rcx
 adc     rax, rax
 ret

break2:
 inc     rcx
 jz      fin2

copy1:
 mov     rax, [rsi+rcx*8]
 mov     [rdi+rcx*8], rax
 inc     rcx
 jnz     copy1

 mov     rax, rcx
 adc     rax, rax
 ret

; bigint_word_t _bigint_sub(bigint_word_t *res, const bigint_word_t *a, const bigint_word_t *b, int size);
; res  = rdi
; a    = rsi
; b    = rdx
; size = rcx

 align 16
 global _bigint_sub

_bigint_sub:
 lea     rdi, [rdi+rcx*8]
 lea     rsi, [rsi+rcx*8]
 lea     rdx, [rdx+rcx*8]
 neg     rcx
 xor     rax, rax

 align 16

loop3:
 mov     rax, [rsi+rcx*8]
 sbb     rax, [rdx+rcx*8]
 mov     [rdi+rcx*8], rax
 inc     rcx
 jnz     loop3

 mov     rax, rcx
 adc     rax, rax
 ret

; bigint_word_t _bigint_subw(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = rdi
; a    = rsi
; w    = rdx
; size = rcx

 align 16
 global _bigint_subw

_bigint_subw:
 lea     rdi, [rdi+rcx*8]
 lea     rsi, [rsi+rcx*8]
 neg     rcx
 xor     rax, rax

 align 16

loop4:
 mov     rax, [rsi+rcx*8]
 sbb     rax, rdx
 mov     [rdi+rcx*8], rax
 jnc     break4
 mov     rdx, 0
 inc     rcx
 jnz     loop4

fin3:
 mov     rax, rcx
 adc     rax, rax
 ret

break4:
 inc     rcx
 jz      fin3

copy2:
 mov     rax, [rsi+rcx*8]
 mov     [rdi+rcx*8], rax
 inc     rcx
 jnz     copy2

 mov     rax, rcx
 adc     rax, rax
 ret

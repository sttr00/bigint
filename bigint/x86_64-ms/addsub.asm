 segment .text

; bigint_word_t _bigint_add(bigint_word_t *res, const bigint_word_t *a, const bigint_word_t *b, int size);
; res  = rcx
; a    = rdx
; b    = r8
; size = r9

 align 16
 global _bigint_add

_bigint_add:
 lea     rcx, [rcx+r9*8]
 lea     rdx, [rdx+r9*8]
 lea     r8, [r8+r9*8]
 neg     r9
 xor     rax, rax

 align 16

loop1:
 mov     rax, [rdx+r9*8]
 adc     rax, [r8+r9*8]
 mov     [rcx+r9*8], rax
 inc     r9
 jnz     loop1

 mov     rax, r9
 adc     rax, rax
 ret

; bigint_word_t _bigint_addw(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = rcx
; a    = rdx
; w    = r8
; size = r9

 align 16
 global _bigint_addw

_bigint_addw:
 lea     rcx, [rcx+r9*8]
 lea     rdx, [rdx+r9*8]
 neg     r9
 xor     rax, rax

 align 16

loop2:
 mov     rax, [rdx+r9*8]
 adc     rax, r8
 mov     [rcx+r9*8], rax
 jnc     break2
 mov     r8, 0
 inc     r9
 jnz     loop2

fin2:
 mov     rax, r9
 adc     rax, rax
 ret

break2:
 inc     r9
 jz      fin2

copy1:
 mov     rax, [rdx+r9*8]
 mov     [rcx+r9*8], rax
 inc     r9
 jnz     copy1

 mov     rax, r9
 adc     rax, rax
 ret

; bigint_word_t _bigint_sub(bigint_word_t *res, const bigint_word_t *a, const bigint_word_t *b, int size);
; res  = rcx
; a    = rdx
; b    = r8
; size = r9

 align 16
 global _bigint_sub

_bigint_sub:
 lea     rcx, [rcx+r9*8]
 lea     rdx, [rdx+r9*8]
 lea     r8, [r8+r9*8]
 neg     r9
 xor     rax, rax

 align 16

loop3:
 mov     rax, [rdx+r9*8]
 sbb     rax, [r8+r9*8]
 mov     [rcx+r9*8], rax
 inc     r9
 jnz     loop3

 mov     rax, r9
 adc     rax, rax
 ret

; bigint_word_t _bigint_subw(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = rcx
; a    = rdx
; w    = r8
; size = r9

 align 16
 global _bigint_subw

_bigint_subw:
 lea     rcx, [rcx+r9*8]
 lea     rdx, [rdx+r9*8]
 neg     r9
 xor     rax, rax

 align 16

loop4:
 mov     rax, [rdx+r9*8]
 sbb     rax, r8
 mov     [rcx+r9*8], rax
 jnc     break4
 mov     r8, 0
 inc     r9
 jnz     loop4

fin3:
 mov     rax, r9
 adc     rax, rax
 ret

break4:
 inc     r9
 jz      fin3

copy2:
 mov     rax, [rdx+r9*8]
 mov     [rcx+r9*8], rax
 inc     r9
 jnz     copy2

 mov     rax, r9
 adc     rax, rax
 ret

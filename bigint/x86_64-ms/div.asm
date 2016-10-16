 segment .text

; bigint_word_t _bigint_td_ndiv(const bigint_word_t *u, bigint_word_t v, bigint_word_t v2)
; u  = rcx
; v  = rdx
; v2 = r8

 align 8
 global _bigint_td_ndiv_impl

_bigint_td_ndiv_impl:
 mov     r9, rdx
 mov     rdx, [rcx+16]
 mov     rax, [rcx+8]
 mov     rcx, [rcx]
 cmp     rdx, r9        ; u[2] >= v ?
 jae     overflow
 div     r9
 mov     r10, rax       ; q
 mov     r11, rdx       ; r
repeat:
 mul     r8             ; q * v2
 sub     rax, rcx
 sbb     rdx, r11
 ja      decrease
done:
 mov     rax, r10
 ret
decrease:
 dec     r10
 add     r11, r9
 jc      done
 mov     rax, r10
 jmp     repeat
overflow:
 mov     r10, -1
 add     rax, rdx       ; u[1] + u[2]
 jc      done
 mov     r11, rax
 mov     rax, r10
 jmp     repeat

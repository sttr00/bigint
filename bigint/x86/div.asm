 %include "x86/platform.inc"
 segment .text

; bigint_word_t _bigint_td_ndiv(const bigint_word_t *u, bigint_word_t v, bigint_word_t v2)
; u  = [esp + 4]
; v  = [esp + 8]
; v2 = [esp + 12]

 align 8
 proc _bigint_td_ndiv_impl

 push    ebx
 push    ebp
 push    esi
 mov     ecx, [esp+16]
 mov     ebx, [esp+20]
 mov     edx, [ecx+8]
 mov     eax, [ecx+4]
 mov     ecx, [ecx]
 cmp     edx, ebx       ; u[2] >= v ?
 jae     overflow
 div     ebx
 mov     esi, eax       ; q
 mov     ebp, edx       ; r
repeat:
 mul     dword [esp+24] ; q * v2
 sub     eax, ecx
 sbb     edx, ebp
 ja      decrease
done:
 mov     eax, esi
 pop     esi
 pop     ebp
 pop     ebx
 ret
decrease:
 sub     esi, 1
 add     ebp, ebx
 jc      done
 mov     eax, esi
 jmp     repeat
overflow:
 mov     esi, -1
 add     eax, edx       ; u[1] + u[2]
 jc      done
 mov     ebp, eax
 mov     eax, esi
 jmp     repeat

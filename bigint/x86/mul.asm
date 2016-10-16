 %include "x86/platform.inc"
 segment .text

; void _bigint_mulw(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = [esp + 4]
; a    = [esp + 8]
; w    = [esp + 12]
; size = [esp + 16]

 align 16
 proc _bigint_mulw

 push    ebx
 push    ebp
 push    esi
 push    edi

 mov     edi, [esp+20]
 mov     esi, [esp+24]
 mov     ebx, [esp+28]
 mov     ecx, [esp+32]
 xor     ebp, ebp
 cld

 align 16

loop1:
 lodsd
 mul     ebx
 add     eax, ebp
 adc     edx, 0
 stosd
 mov     ebp, edx
 dec     ecx
 jnz     loop1

 mov     [edi], ebp

 pop     edi
 pop     esi
 pop     ebp
 pop     ebx
 ret

; void _bigint_muladdw(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = [esp + 4]
; a    = [esp + 8]
; w    = [esp + 12]
; size = [esp + 16]

 align 16
 proc _bigint_muladdw

 push    ebx
 push    ebp
 push    esi
 push    edi

 mov     edi, [esp+20]
 mov     esi, [esp+24]
 mov     ebx, [esp+28]
 mov     ecx, [esp+32]
 xor     ebp, ebp
 cld

 align 16

loop2:
 lodsd
 mul     ebx 
 add     eax, ebp
 adc     edx, 0
 add     eax, [edi]
 adc     edx, 0
 stosd
 mov     ebp, edx
 dec     ecx
 jnz     loop2

 mov     [edi], ebp

 pop     edi
 pop     esi
 pop     ebp
 pop     ebx
 ret

; bigint_word_t _bigint_mulsubw(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = [esp + 4]
; a    = [esp + 8]
; w    = [esp + 12]
; size = [esp + 16]

 align 16
 proc _bigint_mulsubw

 push    ebx
 push    ebp
 push    esi
 push    edi

 mov     edi, [esp+20]
 mov     esi, [esp+24]
 mov     ebx, [esp+28]
 mov     ecx, [esp+32]
 xor     ebp, ebp
 cld

 align 16

loop3:
 lodsd
 mul     ebx 
 add     eax, ebp
 adc     edx, 0
 sub     [edi], eax
 adc     edx, 0
 lea     edi, [edi+4]
 mov     ebp, edx
 dec     ecx
 jnz     loop3

 sub     [edi], ebp
 mov     eax, ecx
 adc     eax, eax

 pop     edi
 pop     esi
 pop     ebp
 pop     ebx
 ret

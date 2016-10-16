 %include "x86/platform.inc"
 segment .text

; void _bigint_mulw_mulx(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = [esp + 4]
; a    = [esp + 8]
; w    = [esp + 12]
; size = [esp + 16]

 align 16
 proc _bigint_mulw_mulx

 push    ebx
 push    ebp
 push    esi
 push    edi

 mov     edi, [esp+20]
 mov     esi, [esp+24]
 mov     edx, [esp+28]
 mov     ecx, [esp+32]
 xor     ebp, ebp
 cld

 align 16

loop1:
 mulx    ebx, eax, [esi]
 adc     eax, ebp
 mov     ebp, ebx
 lea     esi, [esi+4]
 stosd
 dec     ecx
 jnz     loop1

 mov     eax, ebp
 adc     eax, ecx
 mov     [edi], eax

 pop     edi
 pop     esi
 pop     ebp
 pop     ebx
 ret

; void _bigint_muladdw_mulx(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = [esp + 4]
; a    = [esp + 8]
; w    = [esp + 12]
; size = [esp + 16]

 align 16
 proc _bigint_muladdw_mulx

 push    ebx
 push    ebp
 push    esi
 push    edi

 mov     edi, [esp+20]
 mov     esi, [esp+24]
 mov     edx, [esp+28]
 mov     ecx, [esp+32]
 xor     ebp, ebp

 align 16

loop2:
 mulx    ebx, eax, [esi]
 adc     eax, ebp
 adc     ebx, 0
 add     [edi], eax
 lea     esi, [esi+4]
 lea     edi, [edi+4]
 mov     ebp, ebx
 dec     ecx
 jnz     loop2

 mov     eax, ebp
 adc     eax, ecx
 mov     [edi], eax

 pop     edi
 pop     esi
 pop     ebp
 pop     ebx
 ret

; bigint_word_t _bigint_mulsubw_mulx(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = [esp + 4]
; a    = [esp + 8]
; w    = [esp + 12]
; size = [esp + 16]

 align 16
 proc _bigint_mulsubw_mulx

 push    ebx
 push    ebp
 push    esi
 push    edi

 mov     edi, [esp+20]
 mov     esi, [esp+24]
 mov     edx, [esp+28]
 mov     ecx, [esp+32]
 xor     ebp, ebp

 align 16

loop3:
 mulx    ebx, eax, [esi]
 adc     eax, ebp
 adc     ebx, 0
 sub     [edi], eax
 lea     esi, [esi+4]
 lea     edi, [edi+4]
 mov     ebp, ebx
 dec     ecx
 jnz     loop3

 sbb     [edi], ebp
 mov     eax, ecx
 adc     eax, eax

 pop     edi
 pop     esi
 pop     ebp
 pop     ebx
 ret

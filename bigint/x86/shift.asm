 %include "x86/platform.inc"
 segment .text

; bigint_word_t _bigint_shl(bigint_word_t *res, const bigint_word_t *a, int bits, int size);
; res  = [esp + 4]
; a    = [esp + 8]
; bits = [esp + 12]
; size = [esp + 16]

 align 16
 proc _bigint_shl

 push    ebx
 push    ebp
 push    esi
 push    edi

 mov     edi, [esp+20]
 mov     esi, [esp+24]
 mov     ecx, [esp+28]
 mov     edx, [esp+32]

 xor     ebx, ebx
 cld

 align 16

loop1:
 lodsd
 mov     ebp, eax
 shld    eax, ebx, cl
 dec     edx
 mov     ebx, ebp
 stosd
 jnz     loop1

 xor     eax, eax
 shld    eax, ebx, cl

 pop     edi
 pop     esi
 pop     ebp
 pop     ebx
 ret

; bigint_word_t _bigint_shr(bigint_word_t *res, const bigint_word_t *a, int bits, int size);
; res  = [esp + 4]
; a    = [esp + 8]
; bits = [esp + 12]
; size = [esp + 16]

 align 16
 proc _bigint_shr

 push    ebx
 push    ebp
 push    esi
 push    edi

 mov     edi, [esp+20]
 mov     esi, [esp+24]
 mov     ecx, [esp+28]
 mov     edx, [esp+32]

 xor     ebx, ebx
 std

 dec     edx
 lea     edi, [edi+edx*4]
 lea     esi, [esi+edx*4]
 inc     edx

 align 16

loop2:
 lodsd
 mov     ebp, eax
 shrd    eax, ebx, cl
 dec     edx
 mov     ebx, ebp
 stosd
 jnz     loop2

 xor     eax, eax
 shrd    eax, ebx, cl
 cld                  ; must be cleared

 pop     edi
 pop     esi
 pop     ebp
 pop     ebx
 ret

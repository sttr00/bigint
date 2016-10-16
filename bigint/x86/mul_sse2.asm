 %include "x86/platform.inc"
 segment .text

; void _bigint_mulw_sse2(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = [esp + 4]
; a    = [esp + 8]
; w    = [esp + 12]
; size = [esp + 16]

 align 16
 proc _bigint_mulw_sse2

 push      esi
 push      edi

 mov       edi, [esp+12]
 mov       esi, [esp+16]
 movd      xmm0, [esp+20]
 mov       ecx, [esp+24]

 pxor      xmm2, xmm2  ; carry

 align 16

; mov ebx, 111
; db 0x64, 0x67, 0x90

loop1:
 movd      xmm3, [esi]
 pmuludq   xmm3, xmm0
 paddq     xmm2, xmm3
 movd      [edi], xmm2
 lea       esi, [esi+4]
 lea       edi, [edi+4]
 psrldq    xmm2, 4
 dec       ecx
 jnz       loop1

; mov ebx, 222
; db 0x64, 0x67, 0x90

 movd      [edi], xmm2

 pop       edi
 pop       esi 
 ret

; void _bigint_muladdw_sse2(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = [esp + 4]
; a    = [esp + 8]
; w    = [esp + 12]
; size = [esp + 16]

 align 16
 proc _bigint_muladdw_sse2

 push      esi
 push      edi

 mov       edi, [esp+12]
 mov       esi, [esp+16]
 movd      xmm0, [esp+20]
 mov       ecx, [esp+24]

 pxor      xmm2, xmm2  ; carry

 align 16

; mov ebx, 111
; db 0x64, 0x67, 0x90

loop2:
 movd      xmm3, [esi]
 pmuludq   xmm3, xmm0
 paddq     xmm2, xmm3
 movd      xmm4, [edi]
 paddq     xmm2, xmm4
 movd      [edi], xmm2
 lea       esi, [esi+4]
 lea       edi, [edi+4]
 psrldq    xmm2, 4
 dec       ecx
 jnz       loop2

; mov ebx, 222
; db 0x64, 0x67, 0x90

 movd      [edi], xmm2

 pop       edi
 pop       esi 
 ret

; bigint_word_t _bigint_mulsubw_sse2(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = [esp + 4]
; a    = [esp + 8]
; w    = [esp + 12]
; size = [esp + 16]

 align 16
 proc _bigint_mulsubw_sse2

 push      esi
 push      edi

 mov       edi, [esp+12]
 mov       esi, [esp+16]
 movd      xmm0, [esp+20]
 mov       ecx, [esp+24]

 mov       eax, -1
 pcmpeqd   xmm4, xmm4  ; all ones
 movd      xmm2, eax
 psubq     xmm4, xmm2
 psubq     xmm4, xmm2

 ; xmm2 - one's complement borrow (0xFF..FF -> no borrow)
 ; xmm4 - 0xFF..FE00..01

 align 16

loop3:
 movd      xmm3, [esi]
 pmuludq   xmm3, xmm0
 movd      xmm5, [edi]
 psubq     xmm5, xmm3
 paddq     xmm2, xmm5
 paddq     xmm2, xmm4
 movd      [edi], xmm2
 lea       esi, [esi+4]
 lea       edi, [edi+4]
 psrlq     xmm2, 32
 dec       ecx
 jnz       loop3

 inc       ecx
 movd      esi, xmm2
 add       esi, ecx
 setc      al
 add       [edi], esi
 setc      ah
 xor       al, ah
 movzx     eax, al
 xor       eax, ecx

 pop       edi
 pop       esi
 ret

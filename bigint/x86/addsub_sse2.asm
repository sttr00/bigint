 %include "x86/platform.inc"
 segment .text

; bigint_word_t _bigint_add_sse2(bigint_word_t *res, const bigint_word_t *a, const bigint_word_t *b, int size);
; res  = [esp + 4]
; a    = [esp + 8]
; b    = [esp + 12]
; size = [esp + 16]

 align 16
 proc _bigint_add_sse2

 push    esi
 push    edi

 mov     edx, [esp+12]
 mov     esi, [esp+16]
 mov     edi, [esp+20]
 mov     ecx, [esp+24]
 lea     edx, [edx+ecx*4]
 lea     esi, [esi+ecx*4]
 lea     edi, [edi+ecx*4]
 neg     ecx
 pxor    xmm2, xmm2  ; carry

 align 16

loop1:
 movd    xmm0, [esi+ecx*4]
 movd    xmm1, [edi+ecx*4]
 paddq   xmm0, xmm1
 paddq   xmm2, xmm0
 movd    [edx+ecx*4], xmm2
 psrldq  xmm2, 4
 inc     ecx
 jnz     loop1

 movd    eax, xmm2

 pop     edi
 pop     esi
 ret

; bigint_word_t _bigint_sub_sse2(bigint_word_t *res, const bigint_word_t *a, const bigint_word_t *b, int size);
; res  = [esp + 4]
; a    = [esp + 8]
; b    = [esp + 12]
; size = [esp + 16]

 align 16
 proc _bigint_sub_sse2

 push    esi
 push    edi

 mov     edx, [esp+12]
 mov     esi, [esp+16]
 mov     edi, [esp+20]
 mov     ecx, [esp+24]
 lea     edx, [edx+ecx*4]
 lea     esi, [esi+ecx*4]
 lea     edi, [edi+ecx*4]
 neg     ecx
 pxor    xmm2, xmm2  ; borrow

 align 16

loop3:
 movd    xmm0, [esi+ecx*4]
 movd    xmm1, [edi+ecx*4]
 psubq   xmm0, xmm1
 psubq   xmm0, xmm2
 movd    [edx+ecx*4], xmm0
 movdqa  xmm2, xmm0
 psrlq   xmm2, 63
 inc     ecx
 jnz     loop3

 movd    eax, xmm2

 pop     edi
 pop     esi
 ret

 %include "x86/platform.inc"
 segment .text

; bigint_word_t _bigint_add(bigint_word_t *res, const bigint_word_t *a, const bigint_word_t *b, int size);
; res  = [esp + 4]
; a    = [esp + 8]
; b    = [esp + 12]
; size = [esp + 16]

 align 16
 proc _bigint_add

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
 xor     eax, eax

 align 16

loop1:
 mov     eax, [esi+ecx*4]
 adc     eax, [edi+ecx*4]
 mov     [edx+ecx*4], eax
 inc     ecx
 jnz     loop1

 mov     eax, ecx
 adc     eax, eax

 pop     edi
 pop     esi
 ret

; bigint_word_t _bigint_addw(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = [esp + 4]
; a    = [esp + 8]
; w    = [esp + 12]
; size = [esp + 16]

 align 16
 proc _bigint_addw

 push    esi
 push    edi

 mov     edx, [esp+12]
 mov     esi, [esp+16]
 mov     edi, [esp+20]
 mov     ecx, [esp+24]
 lea     edx, [edx+ecx*4]
 lea     esi, [esi+ecx*4]
 neg     ecx
 xor     eax, eax

 align 16

loop2:
 mov     eax, [esi+ecx*4]
 adc     eax, edi
 mov     [edx+ecx*4], eax
 jnc     break2
 mov     edi, 0
 inc     ecx
 jnz     loop2

fin2:
 mov     eax, ecx
 adc     eax, eax

 pop     edi
 pop     esi
 ret

break2:
 inc     ecx
 jz      fin2

copy1:
 mov     eax, [esi+ecx*4]
 mov     [edx+ecx*4], eax
 inc     ecx
 jnz     copy1
 jmp     fin2

; bigint_word_t _bigint_sub(bigint_word_t *res, const bigint_word_t *a, const bigint_word_t *b, int size);
; res  = [esp + 4]
; a    = [esp + 8]
; b    = [esp + 12]
; size = [esp + 16]

 align 16
 proc _bigint_sub

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
 xor     eax, eax

 align 16

loop3:
 mov     eax, [esi+ecx*4]
 sbb     eax, [edi+ecx*4]
 mov     [edx+ecx*4], eax
 inc     ecx
 jnz     loop3

 mov     eax, ecx
 adc     eax, eax

 pop     edi
 pop     esi
 ret

; bigint_word_t _bigint_subw(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
; res  = [esp + 4]
; a    = [esp + 8]
; w    = [esp + 12]
; size = [esp + 16]

 align 16
 proc _bigint_subw

 push    esi
 push    edi

 mov     edx, [esp+12]
 mov     esi, [esp+16]
 mov     edi, [esp+20]
 mov     ecx, [esp+24]
 lea     edx, [edx+ecx*4]
 lea     esi, [esi+ecx*4]
 neg     ecx
 xor     eax, eax

 align 16

loop4:
 mov     eax, [esi+ecx*4]
 sbb     eax, edi
 mov     [edx+ecx*4], eax
 jnc     break4
 mov     edi, 0
 inc     ecx
 jnz     loop4

fin3:
 mov     eax, ecx
 adc     eax, eax

 pop     edi
 pop     esi
 ret

break4:
 inc     ecx
 jz      fin3

copy2:
 mov     eax, [esi+ecx*4]
 mov     [edx+ecx*4], eax
 inc     ecx
 jnz     copy2
 jmp     fin3

#ifndef __bigint_dispatch_h__
#define __bigint_dispatch_h__

#include "defs.h"
#include "defs_arch.h"

#define declare_addsub(name)  extern bigint_word_t _bigint_##name (bigint_word_t *res, const bigint_word_t *a, const bigint_word_t *b, int size)
#define declare_addsubw(name) extern bigint_word_t _bigint_##name (bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size)
#define declare_shift(name)   extern bigint_word_t _bigint_##name (bigint_word_t *res, const bigint_word_t *a, int bits, int size)
#define declare_mul(name)     extern void _bigint_##name (bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size)
#define declare_mulsub(name)  extern bigint_word_t _bigint_##name (bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size)

#ifdef BIGINT_DISABLE_ASM
#ifndef BIGINT_STATIC_DISPATCH
#define BIGINT_STATIC_DISPATCH
#endif
declare_addsub(add_c);
declare_addsub(sub_c);
declare_addsubw(addw_c);
declare_addsubw(subw_c);
declare_addsubw(mulw_c);
declare_addsubw(muladdw_c);
declare_addsubw(mulsubw_c);
declare_shift(shl_c);
declare_shift(shr_c);
#else
declare_addsubw(addw);
declare_addsubw(subw);
declare_shift(shl);
declare_shift(shr);
#endif

#ifdef BIGINT_STATIC_DISPATCH

#define bigint_init_dispatch_table() ((void) 0)

#if defined(ARCH_X86)
/* x86 */
#if defined(BIGINT_ENABLE_SSE2)
declare_addsub(add_sse2);
declare_addsub(sub_sse2);
declare_mul(mulw_sse2);
declare_mul(muladdw_sse2);
declare_mulsub(mulsubw_sse2);
#define call_add     _bigint_add_sse2
#define call_sub     _bigint_sub_sse2
#define call_mulw    _bigint_mulw_sse2
#define call_muladdw _bigint_muladdw_sse2
#define call_mulsubw _bigint_mulsubw_sse2
#elif defined(BIGINT_ENABLE_BMI2)
declare_mul(mulw_mulx);
declare_mul(muladdw_mulx);
declare_mulsub(mulsubw_mulx);
#define call_mulw    _bigint_mulw_mulx
#define call_muladdw _bigint_muladdw_mulx
#define call_mulsubw _bigint_mulsubw_mulx
#endif

#elif defined(ARCH_X86_64)
/* x86_64 */
#if defined(BIGINT_ENABLE_BMI2)
declare_mul(mulw_mulx);
declare_mul(muladdw_mulx);
declare_mulsub(mulsubw_mulx);
#define call_mulw    _bigint_mulw_mulx
#define call_muladdw _bigint_muladdw_mulx
#define call_mulsubw _bigint_mulsubw_mulx
#endif

#elif defined(ARCH_ARM)
/* ARM */
#if defined(BIGINT_ENABLE_UMAAL)
declare_mul(muladdw_umaal);
#define call_muladdw _bigint_muladdw_umaal
#endif

#endif

#ifdef BIGINT_DISABLE_ASM

#define call_add     _bigint_add_c
#define call_sub     _bigint_sub_c
#define call_addw    _bigint_addw_c
#define call_subw    _bigint_subw_c
#define call_mulw    _bigint_mulw_c
#define call_muladdw _bigint_muladdw_c
#define call_mulsubw _bigint_mulsubw_c
#define call_shl     _bigint_shl_c
#define call_shr     _bigint_shr_c

#else /* BIGINT_DISABLE_ASM not defined */

#define call_addw    _bigint_addw
#define call_subw    _bigint_subw
#define call_shl     _bigint_shl
#define call_shr     _bigint_shr

#ifndef call_add
declare_addsub(add);
#define call_add     _bigint_add
#endif

#ifndef call_sub
declare_addsub(sub);
#define call_sub     _bigint_sub
#endif

#ifndef call_mulw
declare_mul(mulw);
#define call_mulw    _bigint_mulw
#endif

#ifndef call_muladdw
declare_mul(muladdw);
#define call_muladdw _bigint_muladdw
#endif

#ifndef call_mulsubw
declare_mulsub(mulsubw);
#define call_mulsubw _bigint_mulsubw
#endif

#endif /* BIGINT_DISABLE_ASM */

#else /* BIGINT_STATIC_DISPATCH not defined */

struct bigint_dispatch_table
{
 bigint_word_t (*func_add)(bigint_word_t *res, const bigint_word_t *a, const bigint_word_t *b, int size);
 bigint_word_t (*func_sub)(bigint_word_t *res, const bigint_word_t *a, const bigint_word_t *b, int size);
 bigint_word_t (*func_addw)(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
 bigint_word_t (*func_subw)(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
 bigint_word_t (*func_shl)(bigint_word_t *res, const bigint_word_t *a, int bits, int size);
 bigint_word_t (*func_shr)(bigint_word_t *res, const bigint_word_t *a, int bits, int size);
 void (*func_mulw)(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
 void (*func_muladdw)(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
 bigint_word_t (*func_mulsubw)(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size);
};

extern struct bigint_dispatch_table _bigint_dispatch;

void bigint_init_dispatch_table();

#define call_add     (_bigint_dispatch.func_add)
#define call_sub     (_bigint_dispatch.func_sub)
#define call_addw    (_bigint_dispatch.func_addw)
#define call_subw    (_bigint_dispatch.func_subw)
#define call_shl     (_bigint_dispatch.func_shl)
#define call_shr     (_bigint_dispatch.func_shr)
#define call_mulw    (_bigint_dispatch.func_mulw)
#define call_muladdw (_bigint_dispatch.func_muladdw)
#define call_mulsubw (_bigint_dispatch.func_mulsubw)

#endif /* BIGINT_STATIC_DISPATCH */

#endif /* __bigint_dispatch_h__ */

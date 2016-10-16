#ifndef __bigint_defs_arch_h__
#define __bigint_defs_arch_h__

#include "defs_word.h"

/* GCC */
#ifdef __GNUC__

#if defined(__i386__)
#define ARCH_X86
#elif defined(__amd64__)
#define ARCH_X86_64
#endif

#if defined(__GNUC_MINOR__) && (__GNUC__*256 + __GNUC_MINOR__ >= 0x303)

#ifdef ENV_64BIT
#define _bigint_w_clz __builtin_clzl
#define _bigint_w_ctz __builtin_ctzl
#else
#define _bigint_w_clz __builtin_clz
#define _bigint_w_ctz __builtin_ctz
#endif

#endif

#if defined(ARCH_X86) || defined(ARCH_X86_64)

#ifndef _bigint_w_clz
#define _bigint_w_clz _bigint_w_clz_impl

static __inline int _bigint_w_clz_impl(bigint_word_t val) 
{
 int res;
 asm ("bsr%z1 %1, %0\n\t"
#ifdef ARCH_X86
      "xorl $31, %0\n\t"
#else
      "xorq $63, %0\n\t"
#endif
 : "=r"(res) : "rm"(val) : "cc");
 return res;
}

#endif /* _bigint_w_clz */

#ifndef _bigint_w_ctz
#define _bigint_w_ctz _bigint_w_ctz_impl

static __inline int _bigint_w_ctz_impl(bigint_word_t val) 
{
 int res;
 asm ("bsf%z1 %1, %0\n\t" : "=r"(res) : "rm"(val) : "cc");
 return res;
}

#endif /* _bigint_w_ctz */

#define _bigint_ww_mul(a, b, res_lo, res_hi) \
 asm ("mul%z3 %3\n\t" : "=a"(res_lo), "=d"(res_hi) : "%0"(a), "rm"(b) : "cc")

#define _bigint_dw_ndiv(a_lo, a_hi, b, q, r) \
 asm ("div%z4 %4\n\t" : "=a"(q), "=d"(r) : "0"(a_lo), "1"(a_hi), "rm"(b) : "cc")

#ifndef BIGINT_DISABLE_ASM
extern bigint_word_t _bigint_td_ndiv_impl(const bigint_word_t *u, bigint_word_t v, bigint_word_t v2);
#define _bigint_td_ndiv _bigint_td_ndiv_impl
#endif /* BIGINT_DISABLE_ASM */

#elif defined(__arm__)

#define ARCH_ARM

#define _bigint_ww_mul(a, b, res_lo, res_hi) \
 asm ("umull %0, %1, %2, %3\n\t" : "=r"(res_lo), "=r"(res_hi) : "%r"(a), "r"(b))

#ifndef BIGINT_DISABLE_ASM
extern bigint_word_t _bigint_dw_ndiv_impl(bigint_word_t a_lo, bigint_word_t a_hi, bigint_word_t b, bigint_word_t *r);
extern bigint_word_t _bigint_td_ndiv_impl(const bigint_word_t *u, bigint_word_t v, bigint_word_t v2);

#define _bigint_dw_ndiv(a_lo, a_hi, b, q, r) \
 do { (q) = _bigint_dw_ndiv_impl(a_lo, a_hi, b, &(r)); } while (0)

#define _bigint_td_ndiv _bigint_td_ndiv_impl
#endif /* BIGINT_DISABLE_ASM */

#endif

#elif defined(_MSC_VER)

#if defined(_M_IX86)
#define ARCH_X86
#elif defined(_M_X64)
#define ARCH_X86_64
#endif

#include <intrin.h>

#define _bigint_w_clz _bigint_w_clz_impl
#define _bigint_w_ctz _bigint_w_ctz_impl

static __forceinline int _bigint_w_clz_impl(bigint_word_t val)
{
 unsigned long pos;
 #ifdef ENV_64BIT
 _BitScanReverse64(&pos, val);
 return pos ^ 63;
 #else
 _BitScanReverse(&pos, val);
 return pos ^ 31;
 #endif
}

static __forceinline int _bigint_w_ctz_impl(bigint_word_t val)
{
 unsigned long pos;
 #ifdef ENV_64BIT
 _BitScanForward64
 #else
 _BitScanForward
 #endif
 (&pos, val);
 return pos;
}

#ifndef BIGINT_DISABLE_ASM
extern bigint_word_t __fastcall _bigint_ww_mul_impl(bigint_word_t a, bigint_word_t b, bigint_word_t *res_hi);
extern bigint_word_t __fastcall _bigint_dw_ndiv_impl(bigint_word_t a_lo, bigint_word_t a_hi, bigint_word_t b, bigint_word_t *r);
extern bigint_word_t _bigint_td_ndiv_impl(const bigint_word_t *u, bigint_word_t v, bigint_word_t v2);

#define _bigint_ww_mul(a, b, res_lo, res_hi) \
 do { (res_lo) = _bigint_ww_mul_impl(a, b, &(res_hi)); } while (0)

#define _bigint_dw_ndiv(a_lo, a_hi, b, q, r) \
 do { (q) = _bigint_dw_ndiv_impl(a_lo, a_hi, b, &(r)); } while (0)

#define _bigint_td_ndiv _bigint_td_ndiv_impl
#endif /* BIGINT_DISABLE_ASM */

#endif

/* generic */

#ifndef _bigint_ww_mul
extern bigint_word_t _bigint_ww_mul_c(bigint_word_t a, bigint_word_t b, bigint_word_t *res_hi);
#define _bigint_ww_mul(a, b, res_lo, res_hi) \
 do { (res_lo) = _bigint_ww_mul_c(a, b, &(res_hi)); } while (0)
#endif

#ifndef _bigint_dw_ndiv
extern bigint_word_t _bigint_dw_ndiv_c(bigint_word_t a_lo, bigint_word_t a_hi, bigint_word_t b, bigint_word_t *r);
#define _bigint_dw_ndiv(a_lo, a_hi, b, q, r) \
 do { (q) = _bigint_dw_ndiv_c(a_lo, a_hi, b, &(r)); } while (0)
#endif

#ifndef _bigint_td_ndiv
extern bigint_word_t _bigint_td_ndiv_c(const bigint_word_t *u, bigint_word_t v, bigint_word_t v2);
#define _bigint_td_ndiv _bigint_td_ndiv_c
#endif

#ifndef _bigint_w_clz
extern int _bigint_w_clz_c(bigint_word_t val);
#define _bigint_w_clz _bigint_w_clz_c
#endif

#ifndef _bigint_w_ctz
extern int _bigint_w_ctz_c(bigint_word_t val);
#define _bigint_w_ctz _bigint_w_ctz_c
#endif

#endif /* __bigint_defs_arch_h__ */

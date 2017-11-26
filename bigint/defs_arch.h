#ifndef __bigint_defs_arch_h__
#define __bigint_defs_arch_h__

#ifndef BIGINT_GENERIC_ARCH
#include <platform/arch.h>
#endif

#include "defs_word.h"

/* GCC */
#ifdef __GNUC__

#if defined(__GNUC_MINOR__) && (__GNUC__*256 + __GNUC_MINOR__ >= 0x303)

#if BIGINT_WORD_SIZE == 8
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
 bigint_word_t res;
 #ifdef ARCH_X86
 asm ("bsrl %1, %0\n\t"
      "xorl $31, %0\n\t" : "=r"(res) : "rm"(val) : "cc");
 #else
 asm ("bsrq %1, %0\n\t"
      "xorq $63, %0\n\t" : "=r"(res) : "rm"(val) : "cc");
 #endif
 return res;
}

#endif /* _bigint_w_clz */

#ifndef _bigint_w_ctz
#define _bigint_w_ctz _bigint_w_ctz_impl

static __inline int _bigint_w_ctz_impl(bigint_word_t val)
{
 bigint_word_t res;
 #ifdef ARCH_X86
 asm ("bsfl %1, %0\n\t" : "=r"(res) : "rm"(val) : "cc");
 #else
 asm ("bsfq %1, %0\n\t" : "=r"(res) : "rm"(val) : "cc");
 #endif
 return res;
}

#endif /* _bigint_w_ctz */

#ifdef ARCH_X86

#define _bigint_ww_mul(a, b, res_lo, res_hi) \
 asm ("mull %3\n\t" : "=a"(res_lo), "=d"(res_hi) : "%0"(a), "rm"(b) : "cc")

#define _bigint_dw_ndiv(a_lo, a_hi, b, q, r) \
 asm ("divl %4\n\t" : "=a"(q), "=d"(r) : "0"(a_lo), "1"(a_hi), "rm"(b) : "cc")

#else

#define _bigint_ww_mul(a, b, res_lo, res_hi) \
 asm ("mulq %3\n\t" : "=a"(res_lo), "=d"(res_hi) : "%0"(a), "rm"(b) : "cc")

#define _bigint_dw_ndiv(a_lo, a_hi, b, q, r) \
 asm ("divq %4\n\t" : "=a"(q), "=d"(r) : "0"(a_lo), "1"(a_hi), "rm"(b) : "cc")

#endif

#ifndef BIGINT_DISABLE_ASM
extern bigint_word_t _bigint_td_ndiv_impl(const bigint_word_t *u, bigint_word_t v, bigint_word_t v2);
#define _bigint_td_ndiv _bigint_td_ndiv_impl
#endif /* BIGINT_DISABLE_ASM */

#elif defined(ARCH_ARM) || defined(ARCH_ARM64)

#ifdef ARCH_ARM
#define _bigint_ww_mul(a, b, res_lo, res_hi) \
 asm ("umull %0, %1, %2, %3\n\t" : "=r"(res_lo), "=r"(res_hi) : "%r"(a), "r"(b))
#else /* ARM64 */
#define _bigint_ww_mul(a, b, res_lo, res_hi) \
 asm ("mul %0, %2, %3\n\t" \
      "umulh %1, %2, %3\n\t" : "=&r"(res_lo), "=r"(res_hi) : "%r"(a), "r"(b))
#endif

#ifndef BIGINT_DISABLE_ASM
extern bigint_word_t _bigint_dw_ndiv_impl(bigint_word_t a_lo, bigint_word_t a_hi, bigint_word_t b, bigint_word_t *r);
extern bigint_word_t _bigint_td_ndiv_impl(const bigint_word_t *u, bigint_word_t v, bigint_word_t v2);

#define _bigint_dw_ndiv(a_lo, a_hi, b, q, r) \
 do { (q) = _bigint_dw_ndiv_impl(a_lo, a_hi, b, &(r)); } while (0)

#define _bigint_td_ndiv _bigint_td_ndiv_impl
#endif /* BIGINT_DISABLE_ASM */

#endif

#elif defined(_MSC_VER)

#include <intrin.h>

#define _bigint_w_clz _bigint_w_clz_impl
#define _bigint_w_ctz _bigint_w_ctz_impl

static __forceinline int _bigint_w_clz_impl(bigint_word_t val)
{
 unsigned long pos;
 #if BIGINT_WORD_SIZE == 8
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
 #if BIGINT_WORD_SIZE == 8
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

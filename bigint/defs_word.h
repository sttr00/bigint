#ifndef __bigint_defs_word_h__
#define __bigint_defs_word_h__

#include <stdint.h>

#ifndef BIGINT_GENERIC_ARCH
#include <platform/arch.h>
#endif

#if defined(BIGINT_WORD32) && defined(BIGINT_GENERIC_ARCH)
#define BIGINT_WORD_SIZE 4
#elif defined(_WIN32)
#ifdef _WIN64
#define BIGINT_WORD_SIZE 8
#else
#define BIGINT_WORD_SIZE 4
#endif
#elif defined(__GNUC__)
#ifdef __LP64__
#define BIGINT_WORD_SIZE 8
#else
#define BIGINT_WORD_SIZE 4
#endif
#else
#error Unknown environment
#endif

#if BIGINT_WORD_SIZE == 8

typedef uint64_t bigint_word_t;
typedef int64_t  bigint_signed_word_t;
#define BIGINT_WORD_SHIFT 3

#else

typedef uint32_t bigint_word_t;
typedef int32_t  bigint_signed_word_t;
#define BIGINT_WORD_SHIFT 2

#endif

#endif /* __bigint_defs_word_h__ */

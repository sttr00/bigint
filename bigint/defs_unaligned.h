#ifndef __bigint_defs_unaligned_h__
#define __bigint_defs_unaligned_h__

#include <platform/unaligned.h>
#include "defs_word.h"

#ifdef BIGINT_GENERIC_ARCH
#undef ARCH_X86
#undef ARCH_X86_64
#undef ARCH_ARM
#undef ARCH_ARM64
#undef ARCH_PPC
#endif

#if BIGINT_WORD_SIZE == 8

#define bigint_get_unaligned_be get_unaligned64_be
#define bigint_get_unaligned_le get_unaligned64_le
#define bigint_put_unaligned_be put_unaligned64_be
#define bigint_put_unaligned_le put_unaligned64_le

#else

#define bigint_get_unaligned_be get_unaligned32_be
#define bigint_get_unaligned_le get_unaligned32_le
#define bigint_put_unaligned_be put_unaligned32_be
#define bigint_put_unaligned_le put_unaligned32_le

#endif

#endif /* __bigint_defs_unaligned_h__ */

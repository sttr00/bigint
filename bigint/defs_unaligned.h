#ifndef __bigint_defs_unaligned_h__
#define __bigint_defs_unaligned_h__

#include "defs_arch.h"
#include "defs_word.h"
#include <platform/endian_ex.h>

#if !defined(HAVE_UNALIGNED_ACCESS) && !defined(HAVE_NO_UNALIGNED_ACCESS)
#if defined(ARCH_X86) || defined(ARCH_X86_64)
#define HAVE_UNALIGNED_ACCESS
#endif
#endif

#ifdef __GNUC__

#ifndef __INLINE
#define __INLINE __inline
#endif

#ifdef HAVE_UNALIGNED_ACCESS

static __INLINE uint16_t get_unaligned16(const void *p) { return *(const uint16_t *) p; }
static __INLINE uint32_t get_unaligned32(const void *p) { return *(const uint32_t *) p; }
static __INLINE uint64_t get_unaligned64(const void *p) { return *(const uint64_t *) p; }

static __INLINE void put_unaligned16(void *p, uint16_t value) { *(uint16_t *) p = value; }
static __INLINE void put_unaligned32(void *p, uint32_t value) { *(uint32_t *) p = value; }
static __INLINE void put_unaligned64(void *p, uint64_t value) { *(uint64_t *) p = value; }

#else

union __uaccess
{
 uint8_t  u8;
 uint16_t u16;
 uint32_t u32;
 uint64_t u64;
} __attribute__((packed));

static __INLINE uint16_t get_unaligned16(const void *p) { return ((const union __uaccess *) p)->u16; }
static __INLINE uint32_t get_unaligned32(const void *p) { return ((const union __uaccess *) p)->u32; }
static __INLINE uint64_t get_unaligned64(const void *p) { return ((const union __uaccess *) p)->u64; }

static __INLINE void put_unaligned16(void *p, uint16_t value) { ((union __uaccess *) p)->u16 = value; }
static __INLINE void put_unaligned32(void *p, uint32_t value) { ((union __uaccess *) p)->u32 = value; }
static __INLINE void put_unaligned64(void *p, uint64_t value) { ((union __uaccess *) p)->u64 = value; }

#endif

#elif defined(_MSC_VER)

#ifndef __INLINE
#define __INLINE __forceinline
#endif

#ifdef HAVE_UNALIGNED_ACCESS
#define __UNALIGNED
#else
#define __UNALIGNED __unaligned
#endif

static __INLINE uint16_t get_unaligned16(const void *p) { return *(const __UNALIGNED uint16_t *) p; }
static __INLINE uint32_t get_unaligned32(const void *p) { return *(const __UNALIGNED uint32_t *) p; }
static __INLINE uint64_t get_unaligned64(const void *p) { return *(const __UNALIGNED uint64_t *) p; }

static __INLINE void put_unaligned16(void *p, uint16_t value) { *(__UNALIGNED uint16_t *) p = value; }
static __INLINE void put_unaligned32(void *p, uint32_t value) { *(__UNALIGNED uint32_t *) p = value; }
static __INLINE void put_unaligned64(void *p, uint64_t value) { *(__UNALIGNED uint64_t *) p = value; }

#else

#warning Unknown compiler

#ifndef __INLINE
#define __INLINE __inline
#endif

static __INLINE uint16_t get_unaligned16(const void *p) { return *(const uint16_t *) p; }
static __INLINE uint32_t get_unaligned32(const void *p) { return *(const uint32_t *) p; }
static __INLINE uint64_t get_unaligned64(const void *p) { return *(const uint64_t *) p; }

static __INLINE void put_unaligned16(void *p, uint16_t value) { *(uint16_t *) p = value; }
static __INLINE void put_unaligned32(void *p, uint32_t value) { *(uint32_t *) p = value; }
static __INLINE void put_unaligned64(void *p, uint64_t value) { *(uint64_t *) p = value; }

#endif

static __INLINE uint16_t get_unaligned16_swap(const void *p) { return SWAP16(get_unaligned16(p)); }
static __INLINE uint32_t get_unaligned32_swap(const void *p) { return SWAP32(get_unaligned32(p)); }
static __INLINE uint64_t get_unaligned64_swap(const void *p) { return SWAP64(get_unaligned64(p)); }

static __INLINE void put_unaligned16_swap(void *p, uint16_t v) { put_unaligned16(p, SWAP16(v)); }
static __INLINE void put_unaligned32_swap(void *p, uint32_t v) { put_unaligned32(p, SWAP32(v)); }
static __INLINE void put_unaligned64_swap(void *p, uint64_t v) { put_unaligned64(p, SWAP64(v)); }

#ifdef __LITTLE_ENDIAN__
#define get_unaligned16_le get_unaligned16
#define get_unaligned32_le get_unaligned32
#define get_unaligned64_le get_unaligned64
#define put_unaligned16_le put_unaligned16
#define put_unaligned32_le put_unaligned32
#define put_unaligned64_le put_unaligned64
#define get_unaligned16_be get_unaligned16_swap
#define get_unaligned32_be get_unaligned32_swap
#define get_unaligned64_be get_unaligned64_swap
#define put_unaligned16_be put_unaligned16_swap
#define put_unaligned32_be put_unaligned32_swap
#define put_unaligned64_be put_unaligned64_swap
#else
#define get_unaligned16_le get_unaligned16_swap
#define get_unaligned32_le get_unaligned32_swap
#define get_unaligned64_le get_unaligned64_swap
#define put_unaligned16_le put_unaligned16_swap
#define put_unaligned32_le put_unaligned32_swap
#define put_unaligned64_le put_unaligned64_swap
#define get_unaligned16_be get_unaligned16
#define get_unaligned32_be get_unaligned32
#define get_unaligned64_be get_unaligned64
#define put_unaligned16_be put_unaligned16
#define put_unaligned32_be put_unaligned32
#define put_unaligned64_be put_unaligned64
#endif

#ifdef ENV_64BIT

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

#ifndef __bigint_defs_int_h__
#define __bigint_defs_int_h__

#include "defs.h"

#define WORD_BITS (BIGINT_WORD_SIZE*8)
#define WORD_BITS_LOG (BIGINT_WORD_SHIFT+3)

enum
{
 BIGINT_FLAG_OWN_BUFFER = 1
};

struct bigint
{
 bigint_word_t *buf;
 int size;
 int capacity;
 int neg;
 unsigned flags;
};

#endif /* __bigint_defs_int_h__ */

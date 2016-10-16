#include "../defs_int.h"

bigint_word_t _bigint_shl_c(bigint_word_t *res, const bigint_word_t *a, int bits, int size)
{
 bigint_word_t prev = 0;
 int i;
 for (i=0; i<size; i++)
 {
  bigint_word_t next = a[i];
  res[i] = next<<bits | prev;
  prev = next>>(WORD_BITS-bits);
 }
 return prev;
}

bigint_word_t _bigint_shr_c(bigint_word_t *res, const bigint_word_t *a, int bits, int size)
{
 bigint_word_t prev = 0;
 int i;
 for (i=size-1; i>=0; i--)
 {
  bigint_word_t next = a[i];
  res[i] = next>>bits | prev;
  prev = next<<(WORD_BITS-bits);
 }
 return prev;
}

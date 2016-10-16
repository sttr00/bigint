#include "../defs_int.h"

bigint_word_t _bigint_add_c(bigint_word_t *res, const bigint_word_t *a, const bigint_word_t *b, int size)
{
 int i;
 bigint_word_t c = 0;
 for (i=0; i<size; i++)
 {
  bigint_word_t x = a[i];
  bigint_word_t y = b[i];
  res[i] = x + y + c;
  c = ((x & y) | ((x | y) & ~res[i])) >> (WORD_BITS-1);
 }
 return c;
}

bigint_word_t _bigint_addw_c(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size)
{
 int i;
 bigint_word_t c = 0;
 bigint_word_t x = a[0];
 res[0] = x + w;
 c = ((x & w) | ((x | w) & ~res[0])) >> (WORD_BITS-1);
 for (i=1; i<size; i++)
 {
  x = a[i];
  res[i] = x + c;
  c = (x & ~res[i]) >> (WORD_BITS-1);
 }
 return c;
}

bigint_word_t _bigint_sub_c(bigint_word_t *res, const bigint_word_t *a, const bigint_word_t *b, int size)
{
 int i;
 bigint_word_t c = 0;
 for (i=0; i<size; i++)
 {
  bigint_word_t x = a[i];
  bigint_word_t y = b[i];
  res[i] = x - y - c;
  c = ((~x & y) | ((~x | y) & res[i])) >> (WORD_BITS-1);
 }
 return c;
}

bigint_word_t _bigint_subw_c(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size)
{
 int i;
 bigint_word_t c = 0;
 bigint_word_t x = a[0];
 res[0] = x - w;
 for (i=1; i<size; i++)
 {
  bigint_word_t x = a[i];
  res[i] = x - c;
  c = (~x & res[i]) >> (WORD_BITS-1);
 }
 return c;
}

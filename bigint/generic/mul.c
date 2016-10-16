#include "../defs_int.h"
#include "../defs_arch.h"

void _bigint_mulw_c(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size)
{
 bigint_word_t c = 0, hi, lo;
 int i;
 for (i=0; i<size; i++)
 {
  _bigint_ww_mul(a[i], w, lo, hi);
  res[i] = lo + c;
  c = hi + (((lo & c) | ((lo | c) & ~res[i])) >> (WORD_BITS-1));
 }
 res[size] = c;
}

void _bigint_muladdw_c(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size)
{
 bigint_word_t c = 0, hi, lo, b1, b2;
 int i;
 for (i=0; i<size; i++)
 {
  _bigint_ww_mul(a[i], w, lo, hi);
  b1 = lo + c;
  hi += (((lo & c) | ((lo | c) & ~b1)) >> (WORD_BITS-1));
  b2 = res[i];
  res[i] = b2 + b1;
  c = hi + (((b1 & b2) | ((b1 | b2) & ~res[i])) >> (WORD_BITS-1));
 }
 res[size] = c;
}

bigint_word_t _bigint_mulsubw_c(bigint_word_t *res, const bigint_word_t *a, bigint_word_t w, int size)
{
 bigint_word_t c = 0, lo, hi, b1, b2, prev = res[size];
 int i;
 for (i=0; i<size; i++)
 {
  _bigint_ww_mul(a[i], w, lo, hi);
  b1 = lo + c;
  hi += (((lo & c) | ((lo | c) & ~b1)) >> (WORD_BITS-1));
  b2 = res[i];
  res[i] = b2 - b1;
  c = hi + (((~b2 & b1) | ((~b2 | b1) & res[i])) >> (WORD_BITS-1));
 }
 res[size] -= c;
 return res[size] > prev;
}

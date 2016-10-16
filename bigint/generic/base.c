#include "../defs_int.h"
#include "../defs_arch.h"

#define HALFWORD_BITS (WORD_BITS/2)
#define HALFWORD_MAX ((~(bigint_word_t) 0)>>HALFWORD_BITS)

bigint_word_t _bigint_ww_mul_c(bigint_word_t a, bigint_word_t b, bigint_word_t *res_hi)
{
 bigint_word_t v1 = (a & HALFWORD_MAX)*(b & HALFWORD_MAX);
 bigint_word_t v2 = (a & HALFWORD_MAX)*(b >> HALFWORD_BITS);
 bigint_word_t v3 = (a >> HALFWORD_BITS)*(b & HALFWORD_MAX);
 bigint_word_t v4 = (a >> HALFWORD_BITS)*(b >> HALFWORD_BITS);
 v2 += (v1 >> HALFWORD_BITS) + (v3 & HALFWORD_MAX); /* no overflow here */
 v1 = (v1 & HALFWORD_MAX) | (v2 << HALFWORD_BITS);
 *res_hi = v4 + (v2 >> HALFWORD_BITS) + (v3 >> HALFWORD_BITS);
 return v1;
}

#if 0
bigint_word_t _bigint_dw_ndiv_c(bigint_word_t y, bigint_word_t x, bigint_word_t d, bigint_word_t *r)
{
 int bits = WORD_BITS;
 bigint_word_t q = 0;
 while (bits)
 {
  unsigned flag = x>>(WORD_BITS-1);
  x = x<<1 | y>>(WORD_BITS-1);
  y <<= 1;
  q <<= 1;
  if (flag || x >= d)
  {
   q |= 1;
   x -= d;
  }
  bits--;
 }
 qr[0] = q;
 qr[1] = x;
}
#endif

bigint_word_t _bigint_dw_ndiv_c(bigint_word_t y, bigint_word_t x, bigint_word_t d, bigint_word_t *r)
{
 int bits = WORD_BITS;
 bigint_word_t q = 0;
 while (bits)
 {
  bigint_signed_word_t flag = (bigint_signed_word_t) x>>(WORD_BITS-1);
  x = x<<1 | y>>(WORD_BITS-1);
  y <<= 1;
  q <<= 1;
  if ((flag | x) >= d)
  {
   q |= 1;
   x -= d;
  }
  bits--;
 }
 *r = x;
 return q;
}

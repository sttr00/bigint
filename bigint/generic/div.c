#include "../defs_arch.h"
#include <assert.h>

bigint_word_t _bigint_td_ndiv_c(const bigint_word_t *u, bigint_word_t v, bigint_word_t v2)
{
 bigint_word_t q, r;
 if (u[2] >= v)
 {
  bigint_word_t prev = u[1];
  assert(u[2] == v);
  q = ~(bigint_word_t) 0;
  r = prev + u[2];
  if (r < prev) return q;
 } else _bigint_dw_ndiv(u[1], u[2], v, q, r);
 for (;;)
 {
  bigint_word_t prev, t0, t1;
  _bigint_ww_mul(v2, q, t0, t1);
  if (t1 < r) break;
  if (t1 == r && t0 <= u[0]) break;
  q--;
  prev = r;
  r += v;
  if (r < prev) break;
 }
 return q;
}

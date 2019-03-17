#include "bigint.h"
#include "defs_int.h"
#include "defs_unaligned.h"
#include "dispatch.h"
#include <platform/alloca.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_STACK_ALLOC 8192

#ifdef BIGINT_MALLOC_ALIGN

#define _bigint_alloca(size) \
 (void *) (((uintptr_t) alloca(((size)<<BIGINT_WORD_SHIFT) + BIGINT_MALLOC_ALIGN) + BIGINT_MALLOC_ALIGN) & ~((uintptr_t) BIGINT_MALLOC_ALIGN-1))

static __inline bigint_word_t *_bigint_malloc(int words)
{
 uint8_t *p = (uint8_t *) malloc((words<<BIGINT_WORD_SHIFT) + BIGINT_MALLOC_ALIGN) + BIGINT_MALLOC_ALIGN;
 uintptr_t offset = ((uintptr_t) p) & (BIGINT_MALLOC_ALIGN-1);
 p -= offset;
 p[-1] = BIGINT_MALLOC_ALIGN - offset;
 return (bigint_word_t *) p;
}

static __inline void _bigint_free(void *p)
{
 uintptr_t offset;
 if (!p) return;
 offset = ((const uint8_t *) p)[-1];
 free((uint8_t *) p - offset);
}

#else

#define _bigint_alloca(words) alloca((words)<<BIGINT_WORD_SHIFT)

static __inline bigint_word_t *_bigint_malloc(int words)
{
 return (bigint_word_t *) malloc(words<<BIGINT_WORD_SHIFT);
}

#define _bigint_free free

#endif

#define _bigint_alloc_temp(words) \
 ((words) <= (MAX_STACK_ALLOC>>BIGINT_WORD_SHIFT)? _bigint_alloca(words) : _bigint_malloc(words))

#define _bigint_free_temp(p, words) \
 do { if ((words) >= (MAX_STACK_ALLOC>>BIGINT_WORD_SHIFT)) _bigint_free(p); } while (0)

bigint_t bigint_create(int size)
{
 bigint_t num = (bigint_t) malloc(sizeof(*num));
 num->size = num->capacity = size;
 num->neg = 0;
 if (size)
 {
  num->buf = _bigint_malloc(size);
  num->flags = BIGINT_FLAG_OWN_BUFFER;
 } else
 {
  num->buf = NULL;
  num->flags = 0;
 }
 bigint_init_dispatch_table();
 return num;
}

bigint_t bigint_create_word(bigint_word_t w)
{
 bigint_t num = (bigint_t) malloc(sizeof(*num));
 num->size = num->capacity = 1;
 num->neg = 0;
 num->buf = _bigint_malloc(1);
 num->buf[0] = w;
 num->flags = BIGINT_FLAG_OWN_BUFFER;
 bigint_init_dispatch_table();
 return num;
}

bigint_t bigint_create_buf(const bigint_word_t *data, int size, int negative, int copy_data)
{
 bigint_t num = (bigint_t) malloc(sizeof(*num));
 num->neg = !!negative;
 assert(data && size);
 if (copy_data)
 {
  num->buf = _bigint_malloc(size);
  memcpy(num->buf, data, size<<BIGINT_WORD_SHIFT);
  num->size = num->capacity = size;
  num->flags = BIGINT_FLAG_OWN_BUFFER;
 } else
 {
  num->buf = (bigint_word_t *) data;
  num->size = size;
  num->capacity = 0;
  num->flags = 0;
 }
 bigint_init_dispatch_table();
 return num;
}

static __inline void bigint_set_buf(bigint_t num, bigint_word_t *buf, int capacity)
{
 if (num->flags & BIGINT_FLAG_OWN_BUFFER) _bigint_free(num->buf);
 num->buf = buf;
 num->capacity = capacity;
 num->flags |= BIGINT_FLAG_OWN_BUFFER;
}

void bigint_destroy(bigint_t num)
{
 if (!num) return;
 if (num->flags & BIGINT_FLAG_OWN_BUFFER) _bigint_free(num->buf);
 free(num);
}

static void bigint_realloc(bigint_t num, int capacity)
{
 if (num->flags & BIGINT_FLAG_OWN_BUFFER) _bigint_free(num->buf);
 num->buf = _bigint_malloc(capacity);
 num->flags |= BIGINT_FLAG_OWN_BUFFER;
 num->capacity = capacity;
 num->size = 0;
}

static void bigint_ensure_space(bigint_t num, int capacity)
{
 int new_cap;
 if (num->capacity >= capacity) return;
 new_cap = num->capacity << 1;
 if (capacity > new_cap) new_cap = capacity;
 bigint_realloc(num, new_cap);
}

static bigint_word_t *bigint_ensure_space_const(const bigint_t num, int *capacity)
{
 int new_cap;
 if (num->capacity >= *capacity) return num->buf;
 new_cap = num->capacity << 1;
 if (*capacity < new_cap)
  *capacity = new_cap; else new_cap = *capacity;
 return _bigint_malloc(new_cap);
}

void bigint_shrink_to_fit(bigint_t num)
{
 if (num->capacity > num->size)
 {
  if (!num->size)
  {
   if (num->flags & BIGINT_FLAG_OWN_BUFFER)
   {
    _bigint_free(num->buf);
    num->flags ^= BIGINT_FLAG_OWN_BUFFER;
   }
   num->buf = NULL;
   num->capacity = 0;
  } else
  {
   bigint_word_t *new_buf = _bigint_malloc(num->size);
   memcpy(new_buf, num->buf, num->size<<BIGINT_WORD_SHIFT);
   if (num->flags & BIGINT_FLAG_OWN_BUFFER) _bigint_free(num->buf);
   num->buf = new_buf;
   num->capacity = num->size;
   num->flags = BIGINT_FLAG_OWN_BUFFER;
  }
 }
}

void bigint_set_sign(bigint_t num, int neg)
{
 num->neg = neg;
}

int bigint_get_sign(const bigint_t num)
{
 return num->neg;
}

int bigint_cmp_abs(const bigint_t a, const bigint_t b)
{
 int i;
 if (a->size > b->size) return 1;
 if (a->size < b->size) return -1;
 for (i = a->size-1; i >= 0; i--)
  if (a->buf[i] != b->buf[i])
   return a->buf[i] > b->buf[i]? 1 : -1;
 return 0;
}

int bigint_cmp(const bigint_t a, const bigint_t b)
{
 if (a->neg)
 {
  if (b->neg) return bigint_cmp_abs(b, a);
  return -1;
 }
 if (b->neg) return 1;
 return bigint_cmp_abs(a, b);
}

int bigint_cmp_word(const bigint_t a, bigint_word_t w)
{
 if (a->size == 1)
 {
  if (a->neg)
  {
   if (!w && !a->buf[0]) return 0;
   return -1;
  }
  if (a->buf[0] == w) return 0;
  if (a->buf[0] > w) return 1;
  return -1;
 }
 return a->neg? -1 : 1;
}

int bigint_eq_word(const bigint_t a, bigint_word_t w)
{
 if (a->size != 1 || a->neg) return 0;
 return a->buf[0] == w;
}

static __inline void bigint_set_size(bigint_t num, int size)
{
 assert(size > 0);
 while (!num->buf[size-1])
  if (--size == 0)
  {
   num->size = 1;
   num->neg = 0;
   return;
  }
 num->size = size;
}

void bigint_move(bigint_t dest, bigint_t src)
{
 if (dest->flags & BIGINT_FLAG_OWN_BUFFER) _bigint_free(dest->buf); 
 dest->buf = src->buf;
 dest->size = src->size;
 dest->capacity = src->capacity;
 dest->neg = src->neg;
 dest->flags = src->flags;
 src->buf = NULL;
 src->size = src->capacity = 0;
 src->neg = 0;
 src->flags = 0;
}

void bigint_add(bigint_t res, const bigint_t a, const bigint_t b)
{
 int size, dsize, capacity;
 bigint_word_t c, *out;
 if (a->neg == b->neg)
 {
  if (a->size >= b->size)
  {
   size = a->size;
   capacity = size + 1;
   if (res == a || res == b)
   {
    out = bigint_ensure_space_const(res, &capacity);
   } else
   {
    bigint_ensure_space(res, capacity);
    out = res->buf;
   }
   c = call_add(out, a->buf, b->buf, b->size);
   dsize = a->size - b->size;
   if (dsize)
    c = call_addw(out + b->size, a->buf + b->size, c, dsize);
  } else
  {
   size = b->size;
   capacity = size + 1;
   if (res == a || res == b)
   {
    out = bigint_ensure_space_const(res, &capacity);
   } else
   {
    bigint_ensure_space(res, capacity);
    out = res->buf;
   }
   c = call_add(out, a->buf, b->buf, a->size);
   dsize = b->size - a->size;
   c = call_addw(out + a->size, b->buf + a->size, c, dsize);
  }
  if (c) out[size++] = c;
  res->size = size;
  res->neg = a->neg;
  if (res->buf != out) bigint_set_buf(res, out, capacity);
  return;
 }
 if (bigint_cmp_abs(a, b) >= 0)
 {
  capacity = size = a->size;
  if (res == a || res == b)
  {
   out = bigint_ensure_space_const(res, &capacity);
  } else
  {
   bigint_ensure_space(res, capacity);
   out = res->buf;
  }
  res->neg = a->neg;
  c = call_sub(out, a->buf, b->buf, b->size);
  dsize = a->size - b->size;
  if (dsize)
   call_subw(out + b->size, a->buf + b->size, c, dsize);
 } else
 {
  capacity = size = b->size;
  if (res == a || res == b)
  {
   out = bigint_ensure_space_const(res, &capacity);
  } else
  {
   bigint_ensure_space(res, capacity);
   out = res->buf;
  }
  res->neg = b->neg;
  c = call_sub(out, b->buf, a->buf, a->size);
  dsize = b->size - a->size;
  if (dsize)
   call_subw(out + a->size, b->buf + a->size, c, dsize);
 }
 if (res->buf != out) bigint_set_buf(res, out, capacity);
 bigint_set_size(res, size);
}

void bigint_sub(bigint_t res, bigint_t a, bigint_t b)
{
 struct bigint tmp;
 tmp.buf = b->buf;
 tmp.size = b->size;
 tmp.neg = b->neg ^ 1;
 bigint_add(res, a, &tmp);
}

void bigint_addw(bigint_t res, const bigint_t a, bigint_word_t w)
{
 bigint_word_t c, *out;
 int size = a->size, capacity;
 if (a->neg)
 {
  if (size == 1 && a->buf[0] <= w)
  {
   w -= a->buf[0];
   bigint_ensure_space(res, 1);
   res->buf[0] = w;
   res->size = 1;
   res->neg = 0;
   return;
  }
  capacity = size;
  out = bigint_ensure_space_const(res, &capacity);
  call_subw(out, a->buf, w, size);
  if (res->buf != out) bigint_set_buf(res, out, capacity);
  res->neg = 1;
  bigint_set_size(res, size);
  return;
 }
 capacity = size + 1;
 out = bigint_ensure_space_const(res, &capacity);
 c = call_addw(out, a->buf, w, size);
 if (c) out[size++] = c;
 if (res->buf != out) bigint_set_buf(res, out, capacity);
 res->size = size;
 res->neg = 0;
}

void bigint_subw(bigint_t res, const bigint_t a, bigint_word_t w)
{
 bigint_word_t c, *out;
 int size = a->size, capacity;
 if (a->neg)
 {
  capacity = size + 1;
  out = bigint_ensure_space_const(res, &capacity);
  c = call_addw(out, a->buf, w, size);
  if (c) out[size++] = c;
  if (res->buf != out) bigint_set_buf(res, out, capacity);
  res->size = size;
  res->neg = 1;
  return;
 }
 if (size == 1 && a->buf[0] <= w)
 {
  w -= a->buf[0];
  bigint_ensure_space(res, 1);
  res->buf[0] = w;
  res->size = 1;
  res->neg = w? 1 : 0;
  return;
 }
 capacity = size;
 out = bigint_ensure_space_const(res, &capacity);
 call_subw(out, a->buf, w, size);
 if (res->buf != out) bigint_set_buf(res, out, capacity);
 bigint_set_size(res, size);
 res->neg = 0;
}

void bigint_mul(bigint_t res, const bigint_t a, const bigint_t b)
{
 int i, asize, bsize, size;
 const bigint_word_t *abuf, *bbuf;
 bigint_word_t *out;
 abuf = a->buf;
 bbuf = b->buf;
 asize = a->size;
 bsize = b->size;

 size = asize + bsize;
 if (res == a || res == b)
 {
  out = _bigint_malloc(size);
 } else
 {
  bigint_ensure_space(res, size);
  out = res->buf;
 }
 call_mulw(out, abuf, bbuf[0], asize);
 for (i = 1; i < bsize; i++)
  call_muladdw(out + i, abuf, bbuf[i], asize);
 if (res->buf != out) bigint_set_buf(res, out, size);
 res->neg = a->neg ^ b->neg;
 bigint_set_size(res, size);
}

void bigint_mulw(bigint_t res, const bigint_t a, bigint_word_t w)
{
 int size = a->size + 1;
 if (res == a)
 {
  bigint_word_t *out = _bigint_malloc(size);
  call_mulw(out, a->buf, w, a->size);
  bigint_set_buf(res, out, size);
 } else
 {
  bigint_ensure_space(res, size);
  call_mulw(res->buf, a->buf, w, a->size);
 }
 res->neg = a->neg;
 bigint_set_size(res, size);
}

void bigint_copy(bigint_t res, const bigint_t num)
{
 if (res == num) return;
 bigint_ensure_space(res, num->size);
 memcpy(res->buf, num->buf, num->size<<BIGINT_WORD_SHIFT);
 res->size = num->size;
 res->neg = num->neg;
}

void bigint_set_word(bigint_t res, bigint_word_t w)
{
 bigint_ensure_space(res, 1);
 res->buf[0] = w;
 res->size = 1;
 res->neg = 0;
}

bigint_word_t bigint_get_ls_word(const bigint_t num)
{
 return num->buf[0];
}

void bigint_divmod(bigint_t q, bigint_t r, const bigint_t a, const bigint_t b)
{
 int i, bits, rcap, qcap;
 const bigint_word_t *u;
 bigint_word_t v, v2;
 bigint_word_t *btmp, *rbuf, *qbuf;
 assert(r != q);
 if (bigint_cmp_abs(a, b) < 0)
 {
  /* NB: a == q is possible */
  bigint_copy(r, a);
  bigint_set_word(q, 0);
  return;
 }
 v = b->buf[b->size-1];
 assert(v);
 bits = _bigint_w_clz(v);
 rcap = a->size + 1;
 if (r == a || r == b)
 {
  rbuf = _bigint_malloc(rcap);
 } else
 {
  bigint_ensure_space(r, rcap);
  rbuf = r->buf;
 }
 qcap = a->size - b->size + 1;
 if (q == a || q == b)
 {
  qbuf = _bigint_malloc(qcap);
 } else
 {
  bigint_ensure_space(q, qcap);
  qbuf = q->buf;
 }
 if (b->size == 1)
 {
  if (bits)
  {
   rbuf[a->size] = call_shl(rbuf, a->buf, bits, a->size);
   v <<= bits;
  } else
  {
   memcpy(rbuf, a->buf, a->size<<BIGINT_WORD_SHIFT);
   rbuf[a->size] = 0;
  }
  for (i = a->size-1; i >= 0; i--)
   _bigint_dw_ndiv(rbuf[i], rbuf[i+1], v, qbuf[i], rbuf[i]);
  if (r->buf != rbuf) bigint_set_buf(r, rbuf, rcap);
  r->buf[0] >>= bits;
  r->size = 1;
  if (r->buf[0]) r->neg = a->neg; else r->neg = 0;
  if (q->buf != qbuf) bigint_set_buf(q, qbuf, qcap);
  q->neg = a->neg ^ b->neg;
  bigint_set_size(q, qcap);
  return;
 }
 if (bits)
 {
  btmp = _bigint_alloc_temp(b->size);
  call_shl(btmp, b->buf, bits, b->size);
  rbuf[a->size] = call_shl(rbuf, a->buf, bits, a->size);
  v = btmp[b->size-1];
 } else
 {
  memcpy(rbuf, a->buf, a->size<<BIGINT_WORD_SHIFT);
  rbuf[a->size] = 0;
  btmp = b->buf;
 }
 v2 = b->size >= 2? btmp[b->size-2] : 0;
 i = a->size - b->size;
 u = rbuf + a->size - 2;
 while (i >= 0)
 {
  bigint_word_t q = _bigint_td_ndiv(u, v, v2);
  if (call_mulsubw(rbuf + i, btmp, q, b->size))
  {
   call_add(rbuf + i, rbuf + i, btmp, b->size);
   q--;
  }
  qbuf[i--] = q;
  u--;
 }
 if (bits) call_shr(rbuf, rbuf, bits, b->size);
 if (r->buf != rbuf) bigint_set_buf(r, rbuf, rcap);
 r->neg = a->neg;
 bigint_set_size(r, b->size);
 if (q->buf != qbuf) bigint_set_buf(q, qbuf, qcap);
 q->neg = a->neg ^ b->neg;
 bigint_set_size(q, qcap);
 if (btmp != b->buf) _bigint_free_temp(btmp, b->size);
}

void bigint_mod(bigint_t r, const bigint_t a, const bigint_t b)
{
 int i, bits, rcap;
 const bigint_word_t *u;
 bigint_word_t v, v2, q;
 bigint_word_t *btmp, *rbuf;
 if (bigint_cmp_abs(a, b) < 0)
 {
  bigint_copy(r, a);
  return;
 }
 v = b->buf[b->size-1];
 assert(v);
 bits = _bigint_w_clz(v);
 rcap = a->size + 1;
 if (r == b)
 {
  rbuf = _bigint_malloc(rcap);
 } else
 if (r == a)
 {
  if (a->capacity >= rcap) rbuf = r->buf;
   else rbuf = _bigint_malloc(rcap);
 } else
 {
  bigint_ensure_space(r, rcap);
  rbuf = r->buf;
 }
 if (b->size == 1)
 {
  if (bits)
  {
   rbuf[a->size] = call_shl(rbuf, a->buf, bits, a->size);
   v <<= bits;
  } else
  {
   memcpy(rbuf, a->buf, a->size<<BIGINT_WORD_SHIFT);
   rbuf[a->size] = 0;
  }
  for (i = a->size-1; i >= 0; i--)
   _bigint_dw_ndiv(rbuf[i], rbuf[i+1], v, q, rbuf[i]);
  if (r->buf != rbuf) bigint_set_buf(r, rbuf, rcap);
  r->buf[0] >>= bits;
  r->size = 1;
  if (r->buf[0]) r->neg = a->neg; else r->neg = 0;
  return;
 }
 if (bits)
 {
  btmp = _bigint_alloc_temp(b->size);
  call_shl(btmp, b->buf, bits, b->size);
  rbuf[a->size] = call_shl(rbuf, a->buf, bits, a->size);
  v = btmp[b->size-1];
 } else
 {
  memcpy(rbuf, a->buf, a->size<<BIGINT_WORD_SHIFT);
  rbuf[a->size] = 0;
  btmp = b->buf;
 }
 v2 = b->size >= 2? btmp[b->size-2] : 0;
 i = a->size - b->size;
 u = rbuf + a->size - 2;
 while (i >= 0)
 {
  q = _bigint_td_ndiv(u, v, v2);
  if (call_mulsubw(rbuf + i, btmp, q, b->size))
   call_add(rbuf + i, rbuf + i, btmp, b->size);
  u--;
  i--;
 }
 if (bits) call_shr(rbuf, rbuf, bits, b->size);
 if (r->buf != rbuf) bigint_set_buf(r, rbuf, rcap);
 r->neg = a->neg;
 bigint_set_size(r, b->size);
 if (btmp != b->buf) _bigint_free_temp(btmp, b->size);
}

bigint_word_t bigint_modw(const bigint_t a, bigint_word_t b)
{
 int i, bits;
 bigint_word_t *rbuf, q;
 assert(b);
 bits = _bigint_w_clz(b);
 rbuf = _bigint_alloc_temp(a->size + 1);
 if (bits)
 {
  rbuf[a->size] = call_shl(rbuf, a->buf, bits, a->size);
  b <<= bits;
 } else
 {
  memcpy(rbuf, a->buf, a->size<<BIGINT_WORD_SHIFT);
  rbuf[a->size] = 0;
 }
 for (i = a->size-1; i >= 0; i--)
  _bigint_dw_ndiv(rbuf[i], rbuf[i+1], b, q, rbuf[i]);
 return rbuf[0] >> bits;
}

void bigint_mmul(bigint_t res, const bigint_t a, const bigint_t b, const bigint_t m)
{
 bigint_mul(res, a, b);
 bigint_mod(res, res, m);
 if (res->neg) bigint_add(res, res, m);
}

void bigint_madd(bigint_t res, const bigint_t a, const bigint_t b, const bigint_t m)
{
 bigint_add(res, a, b);
 bigint_mod(res, res, m);
 if (res->neg) bigint_add(res, res, m);
}

void bigint_msub(bigint_t res, const bigint_t a, const bigint_t b, const bigint_t m)
{
 bigint_sub(res, a, b);
 bigint_mod(res, res, m);
 if (res->neg) bigint_add(res, res, m);
}

void bigint_mpow(bigint_t res, const bigint_t a, const bigint_t n, const bigint_t m)
{
 int i, first_alloc;
 int req_size = 1 + (a->size<<1);
 int d_in = 4, d_out = 2;
 int p_in = 0;
 bigint_t v[5];
 if (res != a && res != n && res != m)
 {
  bigint_ensure_space(res, req_size);
  v[0] = res;
  first_alloc = 1;
 } else
 {
  v[0] = bigint_create(req_size);
  first_alloc = 0;
 }
 for (i=1; i<4; i++) v[i] = bigint_create(req_size);
 v[4] = a;
 bigint_set_word(v[0], 1);
 for (i=0; i<n->size; i++)
 {
  bigint_word_t w = n->buf[i];
  int count = WORD_BITS;
  while (count)
  {
   if (w & 1)
   {
    bigint_mmul(v[p_in^1], v[p_in], v[d_in], m);
    p_in ^= 1;
   }
   bigint_mmul(v[d_out], v[d_in], v[d_in], m);
   d_in = d_out;
   d_out = d_in ^ 1;
   w >>= 1;
   count--;
  }
 }
 if (res != v[p_in]) bigint_move(res, v[p_in]);
 for (i=first_alloc; i<4; i++) bigint_destroy(v[i]);
}

int bigint_get_bit_count(const bigint_t num)
{
 int size = num->size;
 if (!size || !num->buf[size-1]) return 0;
 return (size << WORD_BITS_LOG) - _bigint_w_clz(num->buf[size-1]);
}

int bigint_get_trailing_zeros(const bigint_t num)
{
 int pos = 0;
 int bits = 0;
 while (pos < num->size)
 {
  if (num->buf[pos])
  {
   bits = _bigint_w_ctz(num->buf[pos]);
   break;
  }
  pos++;
 }
 return bits + (pos << WORD_BITS_LOG);
}

static __inline int is_zero(const bigint_t num)
{
 return num->size == 1 && num->buf[0] == 0;
}

void bigint_gcd_ext(bigint_t gcd, bigint_t u, bigint_t v, const bigint_t a, const bigint_t b)
{
 int i, index = 0, swap = 0;
 bigint_t r[2], utmp[2], vtmp[2], q, tmp;
 bigint_t d1, d2;
 if (bigint_cmp_abs(a, b) > 0)
 {
  d1 = a;
  d2 = b;
 } else
 {
  d1 = b;
  d2 = a;
  swap = 1;
 }
 if (is_zero(d2))
 {
  bigint_copy(gcd, d1);
  bigint_set_word(u, swap^1);
  bigint_set_word(v, swap);
  return;
 }
 r[0] = bigint_create(d2->size);
 r[1] = bigint_create(d2->size);
 utmp[0] = bigint_create_word(swap);
 utmp[1] = bigint_create_word(swap^1);
 vtmp[0] = bigint_create_word(swap^1);
 vtmp[1] = bigint_create_word(swap);
 q = bigint_create(0);
 tmp = bigint_create(0);
 for (;;)
 {
  bigint_divmod(q, r[index], d1, d2);
  if (is_zero(r[index])) break;
  bigint_mul(tmp, utmp[index], q);
  bigint_sub(utmp[index^1], utmp[index^1], tmp);
  bigint_mul(tmp, vtmp[index], q);
  bigint_sub(vtmp[index^1], vtmp[index^1], tmp);
  d1 = d2;
  d2 = r[index];
  index ^= 1;
 }
 bigint_copy(gcd, d2);
 bigint_copy(u, utmp[index]);
 bigint_copy(v, vtmp[index]);
 for (i=0; i<2; i++)
 {
  bigint_destroy(r[i]);
  bigint_destroy(utmp[i]);
  bigint_destroy(vtmp[i]);
 }
 bigint_destroy(q);
 bigint_destroy(tmp);
}

int bigint_minv(bigint_t res, bigint_t a, bigint_t m)
{
 int i, index = 0, swap = 0, rv = 0;
 bigint_t r[2], utmp[2], q, tmp;
 bigint_t d1, d2;
 if (bigint_cmp_abs(a, m) > 0)
 {
  d1 = a;
  d2 = m;
 } else
 {
  d1 = m;
  d2 = a;
  swap = 1;
 }
 if (is_zero(d2))
 {
  bigint_set_word(res, 0);
  return 0;
 }
 r[0] = bigint_create(d2->size);
 r[1] = bigint_create(d2->size);
 utmp[0] = bigint_create_word(swap);
 utmp[1] = bigint_create_word(swap^1);
 q = bigint_create(0);
 tmp = bigint_create(0);
 for (;;)
 {
  bigint_divmod(q, r[index], d1, d2);
  if (is_zero(r[index])) break;
  bigint_mul(tmp, utmp[index], q);
  bigint_sub(utmp[index^1], utmp[index^1], tmp);
  d1 = d2;
  d2 = r[index];
  index ^= 1;
 }
 if (d2->size == 1 && d2->buf[0] == 1)
 {
  if (d2->neg) utmp[index]->neg ^= 1;
  rv = 1;
  if (utmp[index]->neg)
   bigint_add(res, utmp[index], m); else
   bigint_copy(res, utmp[index]);
 } else bigint_set_word(res, 0);
 for (i=0; i<2; i++)
 {
  bigint_destroy(r[i]);
  bigint_destroy(utmp[i]);
 }
 bigint_destroy(q);
 bigint_destroy(tmp);
 return rv;
}

int bigint_get_word_count(const bigint_t num)
{
 return num->size;
}

int bigint_get_byte_count(const bigint_t num)
{
 int result, bits;
 if (!num->buf[num->size-1]) return 1;
 result = num->size<<BIGINT_WORD_SHIFT;
 bits = _bigint_w_clz(num->buf[num->size-1]);
 return result - (bits >> 3);
}

int bigint_get_bytes_be(const bigint_t num, void *buf, int size)
{
 int i, out_size, max, bytes;
 bigint_word_t w = num->buf[num->size-1];
 uint8_t *out = (uint8_t *) buf;
 if (!w)
 {
  if (size < 1) return -1;
  out[0] = 0;
  return 1;
 }
 out_size = (num->size << BIGINT_WORD_SHIFT) - (_bigint_w_clz(w) >> 3);
 if (out_size > size) return -1;
 bytes = out_size & (BIGINT_WORD_SIZE-1);
 max = out_size >> BIGINT_WORD_SHIFT;
 out += out_size;
 for (i = 0; i < max; i++)
 {
  out -= BIGINT_WORD_SIZE;
  bigint_put_unaligned_be(out, num->buf[i]);
 }
 while (bytes)
 {
  *(--out) = (uint8_t) w;
  w >>= 8;
  bytes--;
 }
 return out_size;
}

int bigint_get_bytes_le(const bigint_t num, void *buf, int size)
{
 int i, out_size, max, bytes;
 bigint_word_t w = num->buf[num->size-1];
 uint8_t *out = (uint8_t *) buf;
 if (!w)
 {
  if (size < 1) return -1;
  out[0] = 0;
  return 1;
 }
 out_size = (num->size << BIGINT_WORD_SHIFT) - (_bigint_w_clz(w) >> 3);
 if (out_size > size) return -1;
 bytes = out_size & (BIGINT_WORD_SIZE-1);
 max = out_size >> BIGINT_WORD_SHIFT;
 for (i = 0; i < max; i++)
 {
  bigint_put_unaligned_le(out, num->buf[i]);
  out += BIGINT_WORD_SIZE;
 }
 while (bytes)
 {
  *out++ = (uint8_t) w;
  w >>= 8;
  bytes--;
 }
 return out_size;
}

void bigint_set_bytes_be(bigint_t num, const void *data, int size)
{
 int i, start, nz_size, max, bytes, words;
 const uint8_t *in = (const uint8_t *) data;
 for (start = 0; start < size; start++)
  if (in[start]) break;
 if (start == size)
 {
  bigint_set_word(num, 0);
  return;
 }
 nz_size = size - start;
 words = (nz_size + BIGINT_WORD_SIZE-1) >> BIGINT_WORD_SHIFT;
 bigint_ensure_space(num, words);
 max = nz_size >> BIGINT_WORD_SHIFT;
 bytes = nz_size & (BIGINT_WORD_SIZE-1); 
 in += size;
 for (i = 0; i < max; i++)
 {
  in -= BIGINT_WORD_SIZE;
  num->buf[i] = bigint_get_unaligned_be(in);
 }
 if (bytes)
 {
  bigint_word_t w = 0;
  in -= bytes;
  while (bytes)
  {
   w = w << 8 | *in++;
   bytes--;
  }
  num->buf[max] = w;
 }
 num->size = words;
 num->neg = 0;
}

void bigint_set_bytes_le(bigint_t num, const void *data, int size)
{
 int i, nz_size = size, max, bytes, words;
 const uint8_t *in = (const uint8_t *) data;
 while (nz_size)
 {
  if (in[nz_size-1]) break;
  nz_size--;
 }
 if (!nz_size)
 {
  bigint_set_word(num, 0);
  return;
 }
 words = (nz_size + BIGINT_WORD_SIZE-1) >> BIGINT_WORD_SHIFT;
 bigint_ensure_space(num, words);
 max = nz_size >> BIGINT_WORD_SHIFT;
 bytes = nz_size & (BIGINT_WORD_SIZE-1);
 for (i = 0; i < max; i++)
 {
  num->buf[i] = bigint_get_unaligned_le(in);
  in += BIGINT_WORD_SIZE;
 }
 if (bytes)
 {
  bigint_word_t w = 0;
  in += bytes;
  while (bytes)
  {
   w = w << 8 | *(--in);
   bytes--;
  }
  num->buf[max] = w;
 }
 num->size = words;
 num->neg = 0;
}

bigint_t bigint_create_bytes_be(const void *data, int size)
{
 bigint_t res;
 int i, start, nz_size, max, bytes;
 const uint8_t *in = (const uint8_t *) data;
 for (start = 0; start < size; start++)
  if (in[start]) break;
 if (start == size) return bigint_create_word(0);
 nz_size = size - start;
 res = bigint_create((nz_size + BIGINT_WORD_SIZE-1) >> BIGINT_WORD_SHIFT);
 max = nz_size >> BIGINT_WORD_SHIFT;
 bytes = nz_size & (BIGINT_WORD_SIZE-1);
 in += size;
 for (i = 0; i < max; i++)
 {
  in -= BIGINT_WORD_SIZE;
  res->buf[i] = bigint_get_unaligned_be(in);
 }
 if (bytes)
 {
  bigint_word_t w = 0;
  in -= bytes;
  while (bytes)
  {
   w = w << 8 | *in++;
   bytes--;
  }
  res->buf[max] = w;
 }
 return res;
}

bigint_t bigint_create_bytes_le(const void *data, int size)
{
 bigint_t res;
 int i, nz_size = size, max, bytes;
 const uint8_t *in = (const uint8_t *) data;
 while (nz_size)
 {
  if (in[nz_size-1]) break;
  nz_size--;
 }
 if (!nz_size) return bigint_create_word(0);
 res = bigint_create((nz_size + BIGINT_WORD_SIZE-1) >> BIGINT_WORD_SHIFT);
 max = nz_size >> BIGINT_WORD_SHIFT;
 bytes = nz_size & (BIGINT_WORD_SIZE-1);
 for (i = 0; i < max; i++)
 {
  res->buf[i] = bigint_get_unaligned_le(in);
  in += BIGINT_WORD_SIZE;
 }
 if (bytes)
 {
  bigint_word_t w = 0;
  in += bytes;
  while (bytes)
  {
   w = w << 8 | *(--in);
   bytes--;
  }
  res->buf[max] = w;
 }
 return res;
}

void bigint_rshift(bigint_t res, const bigint_t num, int bits)
{
 int words = bits >> WORD_BITS_LOG;
 int size = num->size - words;
 if (words)
 {
  bits &= WORD_BITS-1;
  if (size <= 0)
  {
   bigint_set_word(res, 0);
   return;
  }
  if (res != num)
  {
   bigint_ensure_space(res, size);
   call_shr(res->buf, num->buf + words, bits, size);
  } else
  {
   int i;
   for (i = 0; i < size; i++) res->buf[i] = res->buf[i + words];
   call_shr(res->buf, res->buf, bits, size);
  }
 } else
 {
  if (res != num) bigint_ensure_space(res, size);
  call_shr(res->buf, num->buf, bits, size);
 }
 res->neg = num->neg;
 bigint_set_size(res, size);
}

void bigint_lshift(bigint_t res, const bigint_t num, int bits)
{
 int words = bits >> WORD_BITS_LOG;
 int capacity = num->size + words + 1;
 bigint_word_t *out = bigint_ensure_space_const(res, &capacity);
 if (words)
 {
  int i;
  bits &= WORD_BITS-1;
  if (num->buf != out)
  {
   out[num->size + words] = call_shl(out + words, num->buf, bits, num->size);
  } else
  {
   for (i = num->size-1; i >= 0; i--) out[i + words] = num->buf[i];
   out[num->size + words] = call_shl(out + words, out + words, bits, num->size);
  }
  for (i = 0; i < words; i++) out[i] = 0;
 } else out[num->size] = call_shl(out, num->buf, bits, num->size);
 if (res->buf != out) bigint_set_buf(res, out, capacity);
 res->neg = num->neg;
 bigint_set_size(res, num->size + words + 1); 
}

int bigint_get_bit(const bigint_t num, unsigned index)
{
 unsigned bit = index & (WORD_BITS-1);
 index >>= WORD_BITS_LOG;
 if ((int) index >= num->size) return 0;
 return (num->buf[index] >> bit) & 1;
}

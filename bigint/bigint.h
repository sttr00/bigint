#ifndef __bigint_h__
#define __bigint_h__

#include "defs.h"

#ifdef __cplusplus
extern "C"
{
#endif

bigint_t bigint_create(int size);
bigint_t bigint_create_word(bigint_word_t w);
bigint_t bigint_create_buf(const bigint_word_t *data, int size, int negative, int copy_data);
bigint_t bigint_create_bytes_be(const void *data, int size);
bigint_t bigint_create_bytes_le(const void *data, int size);
void bigint_destroy(bigint_t num);
void bigint_copy(bigint_t res, const bigint_t num);
void bigint_move(bigint_t res, bigint_t num);
void bigint_shrink_to_fit(bigint_t num);

int bigint_get_word_count(const bigint_t num);
int bigint_get_byte_count(const bigint_t num);
int bigint_get_bit_count(const bigint_t num);

int bigint_get_bytes_be(const bigint_t num, void *buf, int size);
int bigint_get_bytes_le(const bigint_t num, void *buf, int size);

void bigint_set_bytes_be(bigint_t num, const void *data, int size);
void bigint_set_bytes_le(bigint_t num, const void *data, int size);

void bigint_set_word(bigint_t res, bigint_word_t w);
void bigint_set_sign(bigint_t num, int neg);
int  bigint_get_sign(const bigint_t num);

int bigint_cmp_abs(const bigint_t a, const bigint_t b);
int bigint_cmp(const bigint_t a, const bigint_t b);
int bigint_cmp_word(const bigint_t a, bigint_word_t w);
int bigint_eq_word(const bigint_t a, bigint_word_t w);

int bigint_get_trailing_zeros(const bigint_t num);
bigint_word_t bigint_get_ls_word(const bigint_t num);

void bigint_add(bigint_t res, const bigint_t a, const bigint_t b);
void bigint_sub(bigint_t res, bigint_t a, bigint_t b);
void bigint_addw(bigint_t res, const bigint_t a, bigint_word_t w);
void bigint_subw(bigint_t res, const bigint_t a, bigint_word_t w);
void bigint_mul(bigint_t res, const bigint_t a, const bigint_t b);
void bigint_mulw(bigint_t res, const bigint_t a, bigint_word_t w);
void bigint_divmod(bigint_t q, bigint_t r, const bigint_t a, const bigint_t b);
void bigint_mod(bigint_t r, const bigint_t a, const bigint_t b);
void bigint_mmul(bigint_t res, const bigint_t a, const bigint_t b, const bigint_t m);
void bigint_mpow(bigint_t res, const bigint_t a, const bigint_t n, const bigint_t m);
void bigint_madd(bigint_t res, const bigint_t a, const bigint_t b, const bigint_t m);
void bigint_msub(bigint_t res, const bigint_t a, const bigint_t b, const bigint_t m);

void bigint_gcd_ext(bigint_t gcd, bigint_t u, bigint_t v, const bigint_t a, const bigint_t b);
int bigint_minv(bigint_t res, bigint_t a, bigint_t m);

void bigint_rshift(bigint_t res, const bigint_t num, int bits);
void bigint_lshift(bigint_t res, const bigint_t num, int bits);

int bigint_get_bit(const bigint_t num, unsigned index);

#ifdef __cplusplus
}
#endif

#endif /* __bigint_h__ */

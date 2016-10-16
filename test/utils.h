#ifndef __utils_h__
#define __utils_h__

#include <bigint/bigint.h>

void print_bytes(const bigint_t num);
void print_bytes_le(const bigint_t num);
bigint_t from_dec(const char *data);
char *to_dec(const bigint_t num, int *ppos);
void print_dec(const bigint_t num);
bigint_t create_random(uint32_t seed, int bytes);

#endif /* __utils_h__ */

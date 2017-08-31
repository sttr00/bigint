#include "utils.h"
#include <platform/alloca.h>
#include <platform/endian_ex.h>
#include "rand32.h"
#include <stdio.h>
#include <string.h>

void print_bytes(const bigint_t num)
{
 int j;
 int size = bigint_get_byte_count(num);
 uint8_t *out = (uint8_t *) alloca(size);
 bigint_get_bytes_be(num, out, size);
 for (j=0; j<size; j++) printf("%02X", out[j]);
 putchar('\n');
}

void print_bytes_le(const bigint_t num)
{
 int j;
 int size = bigint_get_byte_count(num);
 uint8_t *out = (uint8_t *) alloca(size);
 bigint_get_bytes_le(num, out, size);
 for (j=0; j<size; j++) printf("%02X", out[j]);
 putchar('\n');
}

#if 0
static void print_words(const bigint_word_t *w, int size)
{
 int i;
 for (i=size-1; i>=0; i--) printf("%08X ", w[i]);
 putchar('\n');
}
#endif

bigint_t from_dec(const char *data)
{
 int i, len = strlen(data);
 bigint_t res = bigint_create_word(0);
 bigint_t ten = bigint_create_word(10);
 for (i=0; i<len; i++)
  if (data[i] >= '0' && data[i] <= '9')
  {
   bigint_mul(res, res, ten);
   bigint_addw(res, res, data[i]-'0');
  }
 bigint_destroy(ten);
 return res;
}

#define LOG2_10 (0.30102999566398119521373889472449)

char *to_dec(const bigint_t num, int *ppos)
{
 bigint_t q, r, ten;
 bigint_t d = num;
 int sign = bigint_get_sign(num);
 int digits = 1 + (int)(bigint_get_bit_count(num)*LOG2_10) + sign;
 int pos = digits - 1;
 char *buf = (char *) malloc(digits + 1);
 buf[digits] = 0;
 q = bigint_create(0);
 r = bigint_create(0);
 ten = bigint_create_word(10);
 for (;;)
 {
  bigint_divmod(q, r, d, ten);
  buf[pos] = (char) bigint_get_ls_word(r) + '0';
  if (bigint_eq_word(q, 0)) break;
  d = q;
  pos--;
 }
 bigint_destroy(ten);
 bigint_destroy(r);
 bigint_destroy(q);
 if (sign) buf[--pos] = '-';
 *ppos = pos;
 return buf;
}

void print_dec(const bigint_t num)
{
 int pos;
 char *buf = to_dec(num, &pos);
 puts(buf + pos);
 free(buf);
}

bigint_t create_random(uint32_t seed, int bytes)
{
 bigint_t num;
 uint8_t *buf = malloc(bytes);
 int pos = 0;
 while (bytes - pos >= 4)
 {
  uint32_t val = rand32(&seed);
  *(uint32_t *) (buf + pos) = VALUE_LE32(val);
  pos += 4;
 }
 if (pos < bytes)
 {
  uint32_t val = rand32(&seed);
  while (pos < bytes)
  {
   buf[pos++] = val >> 24;
   val <<= 8;
  }
 }
 num = bigint_create_bytes_be(buf, bytes);
 free(buf);
 return num;
}

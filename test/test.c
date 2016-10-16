#include "utils.h"
#include <stdio.h>
#include <assert.h>

static void test_div1()
{
 static const bigint_word_t wmax = ~(bigint_word_t) 0;
 static const bigint_word_t whigh = (bigint_word_t) 1 << (sizeof(bigint_word_t)*8-1);
 bigint_word_t buf1[] = { 0, 0, wmax, whigh-1 };
 bigint_word_t buf2[] = { whigh, whigh };
 bigint_word_t buf3[] = { 0, 0, whigh, whigh-1 };
 bigint_word_t buf4[] = { 1, 0, whigh };
 bigint_word_t buf5[] = { 0, 0x364B, wmax-0xEB };
 bigint_word_t buf6[] = { wmax, wmax-0x7A };
 bigint_word_t *udata[3] = { buf1, buf3, buf5 };
 bigint_word_t *vdata[3] = { buf2, buf4, buf6 };
 int ucount[3] = { 4, 4, 3 };
 int vcount[3] = { 2, 3, 2 };
 int i;
 bigint_t u, v, q, r, x, y;

 q = bigint_create(0);
 r = bigint_create(0);
 x = bigint_create(0);
 y = bigint_create(0);

 for (i=0; i<3; i++)
 {
  u = bigint_create_buf(udata[i], ucount[i], 0, 0);
  v = bigint_create_buf(vdata[i], vcount[i], 0, 0);
  fflush(stdout);
  bigint_divmod(q, r, u, v);
  print_bytes(q);
  print_bytes(r);
  bigint_mul(x, q, v);
  bigint_add(y, x, r);
  printf("%d\n", bigint_cmp(y, u));
  bigint_destroy(u);
  bigint_destroy(v);
 }
}

static void test_div2()
{
 bigint_t u, v, q, r;
 int k, res;
 for (k=0; k<1000; k++)
 {
  uint32_t seed;
  u = create_random(k, 40);
  seed = k + 10;
  v = create_random(seed, 16);
  q = bigint_create(0);
  r = bigint_create(0);
  bigint_divmod(q, r, u, v);
  print_bytes(u);
  print_bytes(v);
  print_bytes(q);
  print_bytes(r);
  bigint_mul(q, q, v);
  bigint_add(q, q, r);
  res = bigint_cmp(u, q);
  printf("%d | %d\n", k, res);
  assert(res == 0);
  bigint_destroy(u);
  bigint_destroy(v);
  bigint_destroy(q);
  bigint_destroy(r);
 }
}

static const char *big_prime = "13,346,329,831,506,706,293,257,326,386,247,629,265,921,500,520,598,741,459,621,075,679,508,555,820,237,803,244,236,345,719,347,004,469,560,250,934,702,957,385,092,960,868,830,341,405,078,055,254,058,198,591";

static void test_pow1()
{
 bigint_t a = bigint_create(0);
 bigint_t b = bigint_create(0);
 bigint_t m = bigint_create(0);
 bigint_t res = bigint_create(0);
 bigint_set_word(a, 3);
 bigint_set_word(b, 1000);
 bigint_set_word(m, 100);
 bigint_mpow(res, a, b, m);
 print_bytes(res);
}

static void test_pow2()
{
 bigint_t a = bigint_create(0);
 bigint_t p = from_dec(big_prime);
 bigint_t p1 = bigint_create(0);
 bigint_t res = bigint_create(0);
 bigint_subw(p1, p, 1);
 bigint_set_word(a, 20000);
 bigint_mpow(res, a, p1, p);
 print_bytes(res);
}

static void test_gcd()
{
 bigint_t a = create_random(243, 17);
 bigint_t b = from_dec(big_prime);
 bigint_t u = bigint_create(0);
 bigint_t v = bigint_create(0);
 bigint_t g = bigint_create(0);
 bigint_t t1 = bigint_create(0);
 bigint_t t2 = bigint_create(0);
 bigint_gcd_ext(g, u, v, a, b);
 printf("g = "); print_dec(g);
 printf("u = "); print_dec(u);
 printf("v = "); print_dec(v);
 bigint_mul(t1, u, a);
 bigint_mul(t2, v, b);
 bigint_add(t1, t1, t2);
 printf("check = "); print_dec(t1);
 bigint_destroy(t2);
 bigint_destroy(t1);
 bigint_destroy(g);
 bigint_destroy(v);
 bigint_destroy(u);
 bigint_destroy(b);
 bigint_destroy(a);
}

static void test_inv()
{
 bigint_t m = from_dec(big_prime);
 bigint_t a = bigint_create_word(2023);
 bigint_t ai = bigint_create(0);
 int rv = bigint_minv(ai, a, m);
 if (rv)
 {
  bigint_t tmp = bigint_create(0);
  printf("inv = "); print_dec(ai);
  bigint_mmul(tmp, a, ai, m);
  printf("check = "); print_dec(tmp);
  bigint_destroy(tmp);
 } else puts("No inverse");
 bigint_destroy(ai);
 bigint_destroy(a);
 bigint_destroy(m);
}

void run_test_base()
{
 test_div1();
}

void run_test_bigint()
{
 test_div2();
 test_pow1();
 test_pow2();
 test_gcd();
 test_inv();
}

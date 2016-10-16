#include "utils.h"
#include "clock.h"
#include <stdio.h>

#define NUM_COUNT 1000
#define CYCLES 100

void benchmark_mul()
{ 
 #define SIZE1 2048
 #define SIZE2 1024
 bigint_t a[NUM_COUNT];
 bigint_t b[NUM_COUNT];
 bigint_t res[NUM_COUNT];
 int i, j;
 int64_t tstart, tend;
 for (i=0; i<NUM_COUNT; i++)
 {
  a[i] = create_random(i+1, SIZE1);
  b[i] = create_random(0x80000000+i, SIZE2);
  res[i] = bigint_create(SIZE1 + SIZE2);
 }
 tstart = get_clock();
 for (j=0; j<CYCLES; j++)
  for (i=0; i<NUM_COUNT; i++)
   bigint_mul(res[i], a[i], b[i]);
 tend = get_clock();
 for (i=0; i<NUM_COUNT; i++)
 {
  #if 0
  print_bytes(res[i]);
  #endif
  bigint_destroy(a[i]);
  bigint_destroy(b[i]);
  bigint_destroy(res[i]);
 }
 printf("mul time: %d\n", clock_diff_to_msec(tstart, tend, get_clock_frequency()));
 #undef SIZE1
 #undef SIZE2
}

void benchmark_div()
{ 
 #define SIZE1 2048
 #define SIZE2 1024
 bigint_t a[NUM_COUNT];
 bigint_t b[NUM_COUNT];
 bigint_t res[NUM_COUNT];
 int i, j;
 int64_t tstart, tend;
 for (i=0; i<NUM_COUNT; i++)
 {
  a[i] = create_random(i+1, SIZE1);
  b[i] = create_random(0x80000000+i, SIZE2);
  res[i] = bigint_create(SIZE2);
 }
 tstart = get_clock();
 for (j=0; j<CYCLES; j++)
  for (i=0; i<NUM_COUNT; i++)
   bigint_mod(res[i], a[i], b[i]);
 tend = get_clock();
 for (i=0; i<NUM_COUNT; i++)
 {
  #if 0
  print_bytes(res[i]);
  #endif
  bigint_destroy(a[i]);
  bigint_destroy(b[i]);
  bigint_destroy(res[i]);
 }
 printf("div time: %d\n", clock_diff_to_msec(tstart, tend, get_clock_frequency()));
 #undef SIZE1
 #undef SIZE2
}

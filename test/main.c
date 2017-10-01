#include "utils.h"
#include "cpu_utils.h"
#include <string.h>
#include <stdio.h>

extern void run_test_base();
extern void run_test_bigint();
extern void benchmark_mul();
extern void benchmark_div();
extern void benchmark_pow();

enum
{
 MODE_TEST_BASE = 1,
 MODE_TEST_BIGINT,
 MODE_BENCHMARK_MUL,
 MODE_BENCHMARK_DIV,
 MODE_BENCHMARK_POW,
 MODE_CPU_INFO
};

int main(int argc, char *argv[])
{
 int i, last_arg = argc-1;
 int mode = 0;
 uint32_t disable_features = 0;

 if (argc < 2)
 {
  printf("Usage: %s [ options ] \n"
         "options:\n"
          "   -t { base | bigint }   run test\n"
          "   -b { mul | div | pow } run benchmark\n"
          "   -cpu                   print cpu features and exit\n"
          "   -x feature             disable cpu feature\n\n", argv[0]);
  return 2;
 }

 for (i=1; i<=last_arg; i++)
  if (!strcmp(argv[i], "-t"))
  {
   if (i == last_arg)
   {
    arg_error:
    fprintf(stderr, "%s: argument required\n", argv[i]);
    return 1;
   }
   ++i;
   if (!strcmp(argv[i], "base")) mode = MODE_TEST_BASE; else
   if (!strcmp(argv[i], "bigint")) mode = MODE_TEST_BIGINT;
   else
   {
    fprintf(stderr, "%s: unknown test\n", argv[i]);
    return 1;
   }
  } else
  if (!strcmp(argv[i], "-b"))
  {
   if (i == last_arg) goto arg_error;
   ++i;
   if (!strcmp(argv[i], "mul")) mode = MODE_BENCHMARK_MUL; else
   if (!strcmp(argv[i], "div")) mode = MODE_BENCHMARK_DIV; else
   if (!strcmp(argv[i], "pow")) mode = MODE_BENCHMARK_POW;
   else
   {
    fprintf(stderr, "%s: unknown benchmark\n", argv[i]);
    return 1;
   }
  } else
  if (!strcmp(argv[i], "-cpu"))
  {
   mode = MODE_CPU_INFO;
  } else
  if (!strcmp(argv[i], "-x"))
  {
   uint32_t feature;
   if (i == last_arg) goto arg_error;
   feature = get_cpu_feature_by_name(argv[++i]);
   if (!feature)
   {
    fprintf(stderr, "%s: unknown cpu feature\n", argv[i]);
    return 1;
   }
   disable_features |= feature;
  } else
  {
   fprintf(stderr, "%s: unknown option\n", argv[i]);
   return 1;
  }

 mask_cpu_features(disable_features);
 switch (mode)
 {
  case MODE_TEST_BASE:
   run_test_base();
   break;

  case MODE_TEST_BIGINT:
   run_test_bigint();
   break;

  case MODE_BENCHMARK_MUL:
   benchmark_mul();
   break;

  case MODE_BENCHMARK_DIV:
   benchmark_div();
   break;

  case MODE_BENCHMARK_POW:
   benchmark_pow();
   break;

  case MODE_CPU_INFO:
   print_cpu_features();
   break;

  default:
   puts("Please specify -t or -b");
   break;
 }

 return 0;
}

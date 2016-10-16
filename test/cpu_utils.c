#include "cpu_utils.h"
#include <string.h>
#include <stdio.h>

struct feature_def
{
 const char *name;
 unsigned flag;
};

static const struct feature_def fd[] =
{
 { "sse",    CPU_FEAT_SSE       }, 
 { "sse2",   CPU_FEAT_SSE2      },
 { "sse3",   CPU_FEAT_SSE3      },
 { "clmul",  CPU_FEAT_PCLMULQDQ },
 { "ssse3",  CPU_FEAT_SSSE3     },
 { "fma",    CPU_FEAT_FMA       },
 { "sse41",  CPU_FEAT_SSE41     },
 { "sse42",  CPU_FEAT_SSE42     },
 { "avx",    CPU_FEAT_AVX       },
 { "rdrand", CPU_FEAT_RDRAND    },
 { "bmi1",   CPU_FEAT_BMI1      },
 { "avx2",   CPU_FEAT_AVX2      },
 { "bmi2",   CPU_FEAT_BMI2      },
 { "rdseed", CPU_FEAT_RDSEED    },
 { "adx",    CPU_FEAT_ADX       },
 { "sha",    CPU_FEAT_SHA       },
 { "neon",   CPU_FEAT_NEON      }
};

uint32_t get_cpu_feature_by_name(const char *name)
{
 int i;
 for (i=0; i<sizeof(fd)/sizeof(fd[0]); i++)
  if (!strcmp(name, fd[i].name)) return fd[i].flag;
 return 0;
}

void print_cpu_features()
{
 uint32_t feat = get_cpu_features();
 int i;
 for (i=0; i<sizeof(fd)/sizeof(fd[0]); i++)
  printf("%-7s: %s\n", fd[i].name, (feat & fd[i].flag)? "yes" : "no");
 putchar('\n');
}

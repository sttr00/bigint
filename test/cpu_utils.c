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
#if defined(ARCH_X86) || defined(ARCH_X86_64)
 { "sse",    CPU_FEAT_X86_SSE       }, 
 { "sse2",   CPU_FEAT_X86_SSE2      },
 { "sse3",   CPU_FEAT_X86_SSE3      },
 { "clmul",  CPU_FEAT_X86_PCLMULQDQ },
 { "ssse3",  CPU_FEAT_X86_SSSE3     },
 { "fma",    CPU_FEAT_X86_FMA       },
 { "sse41",  CPU_FEAT_X86_SSE41     },
 { "sse42",  CPU_FEAT_X86_SSE42     },
 { "popcnt", CPU_FEAT_X86_POPCNT    },
 { "avx",    CPU_FEAT_X86_AVX       },
 { "rdrand", CPU_FEAT_X86_RDRAND    },
 { "bmi1",   CPU_FEAT_X86_BMI1      },
 { "avx2",   CPU_FEAT_X86_AVX2      },
 { "bmi2",   CPU_FEAT_X86_BMI2      },
 { "rdseed", CPU_FEAT_X86_RDSEED    },
 { "adx",    CPU_FEAT_X86_ADX       },
 { "sha",    CPU_FEAT_X86_SHA       }
#elif defined(ARCH_ARM) || defined(ARCH_ARM64)
 { "umaal",  CPU_FEAT_ARM_UMAAL     },
 { "edsp",   CPU_FEAT_ARM_EDSP      },
 { "vfp",    CPU_FEAT_ARM_VFP       },
 { "vfp3",   CPU_FEAT_ARM_VFP3      },
 { "neon",   CPU_FEAT_ARM_NEON      }
#else
 { NULL,     0                      }
#endif
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

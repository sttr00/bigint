#ifndef __cpu_features_h__
#define __cpu_features_h__

#include <stdint.h>

/* x86, x86_64 */
enum
{
 CPU_FEAT_X86_SSE       = 0x00000001,
 CPU_FEAT_X86_SSE2      = 0x00000002,
 CPU_FEAT_X86_SSE3      = 0x00000004,
 CPU_FEAT_X86_PCLMULQDQ = 0x00000008,
 CPU_FEAT_X86_SSSE3     = 0x00000010,
 CPU_FEAT_X86_FMA       = 0x00000020,
 CPU_FEAT_X86_SSE41     = 0x00000040,
 CPU_FEAT_X86_SSE42     = 0x00000080,
 CPU_FEAT_X86_AVX       = 0x00000100,
 CPU_FEAT_X86_RDRAND    = 0x00000200,
 CPU_FEAT_X86_BMI1      = 0x00000400,
 CPU_FEAT_X86_AVX2      = 0x00000800,
 CPU_FEAT_X86_BMI2      = 0x00001000,
 CPU_FEAT_X86_RDSEED    = 0x00002000,
 CPU_FEAT_X86_ADX       = 0x00004000,
 CPU_FEAT_X86_SHA       = 0x00008000,
 CPU_FEAT_X86_POPCNT    = 0x00010000
};

/* arm */
enum
{
 CPU_FEAT_ARM_UMAAL     = 0x00000001,
 CPU_FEAT_ARM_EDSP      = 0x00000002,
 CPU_FEAT_ARM_VFP       = 0x00000004,
 CPU_FEAT_ARM_VFP3      = 0x00000008,
 CPU_FEAT_ARM_NEON      = 0x00000010
};

#ifdef __cplusplus
extern "C"
{
#endif

uint32_t get_cpu_features();
void mask_cpu_features(uint32_t mask);

#ifdef __cplusplus
}
#endif

#endif /* __cpu_features_h__ */

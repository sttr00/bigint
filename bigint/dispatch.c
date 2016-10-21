#include "dispatch.h"

#ifndef BIGINT_STATIC_DISPATCH
#include <cpuid/cpu_features.h>

static int initialized;
struct bigint_dispatch_table _bigint_dispatch;

#define set_func(name) \
 _bigint_dispatch.func_##name = _bigint_##name

#define set_func2(name, suffix) \
 _bigint_dispatch.func_##name = _bigint_##name##_##suffix

void bigint_init_dispatch_table()
{
 uint32_t feat;
 if (initialized) return;

 declare_mul(mulw);
 declare_mul(muladdw);
 declare_mulsub(mulsubw);

 set_func(add);
 set_func(sub);
 set_func(addw);
 set_func(subw);
 set_func(shl);
 set_func(shr);
 set_func(mulw);
 set_func(muladdw);
 set_func(mulsubw);

 feat = get_cpu_features();
 #if defined(ARCH_X86)
 declare_addsub(add_sse2);
 declare_addsub(sub_sse2);
 declare_mul(mulw_sse2);
 declare_mul(muladdw_sse2);
 declare_mulsub(mulsubw_sse2);
 declare_mul(mulw_mulx);
 declare_mul(muladdw_mulx);
 declare_mulsub(mulsubw_mulx);

 #if 0
 if (feat & CPU_FEAT_AVX2)
 {
  set_func2(add, sse2);
  set_func2(sub, sse2);
  set_func2(mulw, avx2);
  set_func2(muladdw, avx2);
  set_func2(mulsubw, sse2);
 } else
 #endif
 if (feat & CPU_FEAT_SSE2)
 {
  set_func2(add, sse2);
  set_func2(sub, sse2);
  set_func2(mulw, sse2);
  set_func2(muladdw, sse2);
  set_func2(mulsubw, sse2);
 } else
 if (feat & CPU_FEAT_BMI2)
 {
  set_func2(mulw, mulx);
  set_func2(muladdw, mulx);
  set_func2(mulsubw, mulx);
 }
 #elif defined(ARCH_X86_64)
 declare_mul(mulw_mulx);
 declare_mul(muladdw_mulx);
 declare_mulsub(mulsubw_mulx);

 if (feat & CPU_FEAT_BMI2)
 {
  set_func2(mulw, mulx);
  set_func2(muladdw, mulx);
  set_func2(mulsubw, mulx);
 }
 #elif defined(ARCH_ARM)
 declare_mul(muladdw_umaal);

 if (feat & CPU_FEAT_UMAAL)
  set_func2(muladdw, umaal);
 #endif

 initialized = 1;
}

#endif /* BIGINT_STATIC_DISPATCH */

#ifndef __cpu_utils_h__
#define __cpu_utils_h__

#include <cpuid/cpu_features.h>

uint32_t get_cpu_feature_by_name(const char *name);
void print_cpu_features();

#endif /* __cpu_utils_h__ */
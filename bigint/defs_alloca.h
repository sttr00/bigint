#ifndef __bigint_defs_alloca_h__
#define __bigint_defs_alloca_h__

#ifdef _WIN32
#include <malloc.h>
#ifndef alloca
#define alloca _alloca
#endif
#else
#include <stdlib.h>
#endif

#endif /* __bigint_defs_alloca_h__ */

#ifndef __clock_h__
#define __clock_h__

#include <stdint.h>

#ifdef _WIN32
#include <windows.h>

static __inline int64_t get_clock()
{
 LARGE_INTEGER li;
 if (!QueryPerformanceCounter(&li)) return 0;
 return li.QuadPart;
}

static __inline int64_t get_clock_frequency()
{
 LARGE_INTEGER li;
 if (!QueryPerformanceFrequency(&li)) return 1;
 return li.QuadPart;
}

#else

#include <time.h>

static __inline int64_t get_clock()
{
 struct timespec t;
 clock_gettime(CLOCK_MONOTONIC, &t);
 return t.tv_sec*1000000000ll + t.tv_nsec;
}

static __inline int64_t get_clock_frequency()
{
 return 1000000000ll;
}

#endif

static __inline int clock_diff_to_msec(int64_t start, int64_t end, int64_t frequency)
{
 int diff = (int) ((end-start)*1000/frequency);
 if (diff < 0) diff = 0;
 return diff;
}

#endif /* __clock_h__ */

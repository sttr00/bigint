#include "rand32.h"

uint32_t rand32(uint32_t *seed)
{
 uint32_t next = *seed;
 uint32_t result;

 next *= 1103515245;
 next += 12345;
 result = (next>>16) & 4095;

 next *= 1103515245;
 next += 12345;
 result <<= 10;
 result ^= (next>>16) & 1023;

 next *= 1103515245;
 next += 12345;
 result <<= 10;
 result ^= (next>>16) & 1023;

 *seed = next;

 return result;
}

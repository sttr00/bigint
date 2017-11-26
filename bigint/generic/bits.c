#include "../defs_int.h"

int _bigint_w_clz_c(bigint_word_t val)
{
 int result = 1;
#if BIGINT_WORD_SIZE == 8
#define HW 32
 if (!(val >> 32)) { result += 32; val <<= 32; }
#else
#define HW 0
#endif
 if (!(val >> (16 + HW))) { result += 16; val <<= 16; }
 if (!(val >> (24 + HW))) { result += 8;  val <<= 8;  }
 if (!(val >> (28 + HW))) { result += 4;  val <<= 4;  }
 if (!(val >> (30 + HW))) { result += 2;  val <<= 2;  }
 return result - (val >> (WORD_BITS-1));
}

int _bigint_w_ctz_c(bigint_word_t val)
{
 int result = 1;
#if BIGINT_WORD_SIZE == 8
 if (!(val & 0xFFFFFFFF)) { result += 32; val >>= 32; }
#endif
 if (!(val & 0x0000FFFF)) { result += 16; val >>= 16; }
 if (!(val & 0x000000FF)) { result += 8;  val >>= 8;  }
 if (!(val & 0x0000000F)) { result += 4;  val >>= 4;  }
 if (!(val & 0x00000003)) { result += 2;  val >>= 2;  }
 return result - (val & 1);
}

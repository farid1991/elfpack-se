#include "rand.h"

static unsigned long next = 1;

int rand(void)
{
     int r;
     next = next * 1103515245 + 12345;
     r = (int)((next/65536) % ((unsigned long)RAND_MAX + 1));
     return r;
}

void srand(unsigned seed)
{
     next = seed;
}

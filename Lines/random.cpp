#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h"
#include "random.h"

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

int random(unsigned num)
{
  return (int)(((long)rand()*(num))/(RAND_MAX+1));
}

void randomize()
{
  DATETIME dt;
  REQUEST_DATEANDTIME_GET(SYNC,&dt);
  srand( 60*60*dt.time.hour + 60*dt.time.min + dt.time.sec) ;
}


#ifndef _RANDOM_H
#define _RANDOM_H

#define RAND_MAX      32767

int rand(void);
void srand(unsigned seed);
int random(unsigned num);
void randomize(void);

#endif //_RANDOM_H


static unsigned long x;

void srand(unsigned long val)
{
	x = ~val;
}

unsigned long xor128()
{
	static unsigned long y=362436069,z=521288629,w=88675123;
	unsigned long t;
	t=(x^(x<<11));x=y;y=z;z=w;
	return(w=(w^(w>>19))^(t^(t>>8)));
}

unsigned long rand()
{
	return xor128();
}

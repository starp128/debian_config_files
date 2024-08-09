
#include <stdio.h>

//#define mb() 	asm volatile("mfence":::"memory")

int main()
{
	mb();

	return 0;
}

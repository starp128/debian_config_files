#include <stdio.h>

int main()
{
	int i;
	
	for (i=0; i<255; i++) {
		printf("%c ", *(char*)&i);
		if (i % 16 == 0) printf("\n");
	}
}

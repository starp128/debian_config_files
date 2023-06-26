#include <stdio.h>
#include <unistd.h>

int main()
{
	int i;
	//int ch = 0x11111111;
	int ch;
	
	for (i=0; i<(1024*1024/4); i++) { /* 1M */
		ch = i;
		write(1, &ch, 4);
	}
}

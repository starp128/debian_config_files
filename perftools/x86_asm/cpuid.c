#include <stdio.h>

int main()
{
	int var1, var2, var3, var4;
	int op = 0;

	asm ("cpuid"
	     : "=a" (var1),
	     "=b" (var2),
	     "=c" (var3),
	     "=d" (var4)
	     : "a" (op));

	printf("%x %x %x %x\n", var1, var2, var3, var4);
}

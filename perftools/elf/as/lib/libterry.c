#include <stdio.h>

int library_funa(int a)
{
	printf("i am called\n");
	return ++a;
}
static int library_funb(int b)
{
	return library_funa(++b);
}
int library_func(int argument)
{
	return library_funb(++argument);
}

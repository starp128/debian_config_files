#include <stdio.h>

__asm__(".symver original_foo,foo@");
__asm__(".symver old_foo,foo@VERS_1.1");
__asm__(".symver old_foo1,foo@VERS_1.2");
__asm__(".symver new_foo,foo@@VERS_2.0");

int original_foo()
{
	printf("original foo\n");
	return 0;
}

int old_foo()
{
	printf("old foo\n");
	return 0;
}

int old_foo1()
{
	printf("old foo1\n");
	return 0;
}

int new_foo()
{
	printf("new foo\n");
	return 0;
}

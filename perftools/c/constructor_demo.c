#include <stdio.h>

void __attribute__ ((constructor)) a_constructor() {
	printf("%s\n", __FUNCTION__);
}

void __attribute__ ((destructor)) a_destructor() {
	printf("%s\n", __FUNCTION__);
}

int
main()
{
	printf("%s\n",__FUNCTION__);
	return 0;
}

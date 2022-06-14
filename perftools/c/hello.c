#include <stdio.h>
#include "hello.h"

#define SHORT_STRING "abc"
#define LONG_STRING "alongstring"

int main()
{
	printf(MA);
	printf("%s\n", SHORT_STRING);
	printf("%8s\n", SHORT_STRING);
	printf("%-8s\n", SHORT_STRING);

	printf("%s\n", LONG_STRING);
	printf("%8.8s\n", LONG_STRING);
	/* with 8. 7 from LONG_STRING, left align */
	printf("%-8.7s\n", LONG_STRING);

	return 0;
}

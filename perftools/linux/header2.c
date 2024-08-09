#include <stdio.h>

#ifndef __KERNEL__
#warn "no"
#else
#warn "yes"
#endif

int main(int argc, char *argv)
{
	return 0;
}

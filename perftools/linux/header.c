/*
# 1 "/usr/include/asm-generic/errno-base.h" 1 3 4
# 6 "/usr/include/asm-generic/errno.h" 2 3 4
# 2 "/usr/include/x86_64-linux-gnu/asm/errno.h" 2 3 4
# 2 "/usr/include/linux/errno.h" 2 3 4
# 27 "/usr/include/x86_64-linux-gnu/bits/errno.h" 2 3 4
# 29 "/usr/include/errno.h" 2 3 4
*/


#include <stdio.h>
//#include <errno.h>  ok
#include <linux/errno.h> //ok

int main(int argc, char **argv)
{
	printf("%d\n", -EIO);
	return 0;
}

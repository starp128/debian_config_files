#include <stdio.h>
#include "debug.h"

int main()
{
	pr_debug("hello world debug %d\n", 1);
	pr_info("hello world info %d\n", 2);
	pr_err("hello world err %d\n", 3);
	return 0;
}

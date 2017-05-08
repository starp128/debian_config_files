#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
 
extern int foo();

//.symver actual, alias@version
//__asm__(".symver realpath,realpath@GLIBC_2.3");
__asm__(".symver my_haha_realpath,realpath@GLIBC_2.3");
__asm__(".symver old_realpath,realpath@GLIBC_2.0");

__asm__(".symver foo,foo@VERS_1.1");

int main()
{
	char* unresolved = "/lib/modules/..";
	char  resolved[PATH_MAX+1];
 
	if(!realpath(unresolved, resolved))
	{ return 1; }

	printf("%s\n", resolved);

	unresolved = "/lib/modules/.";
	
	if(!my_haha_realpath(unresolved, resolved))
	{ return 1; }

	printf("%s\n", resolved);

	unresolved = "/lib/modules/../..";
	
	if(!old_realpath(unresolved, resolved))
	{ return 1; }

	printf("%s\n", resolved);

	foo();
	return 0;
}

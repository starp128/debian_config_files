#include <stdio.h>
#include <dlfcn.h>
#include "run_loop.h"

int main(int argc, char **argv) 
{
	void *lib_handle;
	void (*fn)(int ,int);
	char *error;

	lib_handle = dlopen("/usr/lib/librun_loop.so", RTLD_LAZY);
	if (!lib_handle) 
	{
		fprintf(stderr, "%s\n", dlerror());
		return -1;
	}

	fn = dlsym(lib_handle, "run_loop");
	if ((error = dlerror()) != NULL)  
	{
		fprintf(stderr, "%s\n", error);
		return -2;
	}

	(*fn)(1000000,2000);
	fn = dlsym(lib_handle, "run_another_loop");
	if ((error = dlerror()) != NULL)  
	{
		fprintf(stderr, "%s\n", error);
		return -2;
	}

	(*fn)(1000000,1500);

	printf("done\n");

	dlclose(lib_handle);
	return 0;
}

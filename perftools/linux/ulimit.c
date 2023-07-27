#include <stdio.h>
#include <sys/resource.h>

#include <string.h>
#include <errno.h>

int main(int argc, char** argv)
{
	struct rlimit old_lim, lim, new_lim;

	if (getrlimit(RLIMIT_AS, &old_lim) < 0) {
		printf("getrlimit failed %s\n", strerror(errno));
	}

	printf("Old limits -> soft limit= %ld \t"
	       " hard limit= %ld \n",
	       old_lim.rlim_cur,
	       old_lim.rlim_max);

	new_lim.rlim_cur = 0xffffffff;
	new_lim.rlim_max = 0xffffffff;

	if(setrlimit(RLIMIT_AS, &new_lim) == -1)
		fprintf(stderr, "%s\n", strerror(errno));
      
	if (getrlimit(RLIMIT_AS, &old_lim) < 0) {
		printf("getrlimit failed %s\n", strerror(errno));
	}

	printf("new limits -> soft limit= 0x%lx \t"
	       " hard limit= 0x%lx \n",
	       old_lim.rlim_cur,
	       old_lim.rlim_max);


	return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char** argv)
{
	printf("pid is %d", getpid());
	getchar();
}

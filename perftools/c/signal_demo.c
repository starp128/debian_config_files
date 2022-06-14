#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<errno.h>

void sig_ttou(int signo)
{
	printf("SIGTTOU received\n");
	exit(-1);
}

int main(void)
{
	signal(SIGTTOU, sig_ttou);
	printf("hello\n");
	return 0;
}

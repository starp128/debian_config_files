//#include<time.h>
#include<stdio.h>
#include<sys/time.h>

int main(int argc,char** argv)
{
	time_t a;
	struct timeval tv;
	printf("%d\n",sizeof(time_t));
	time(&a);

	gettimeofday(&tv,NULL);
	//do_gettimeofday(&tv);
	printf("FULL unsigned long %lu\n",(unsigned long)-1);
	printf("FULL 4Byte is %ld\n",0x7fffffff);
	printf("time %ld\n",a);

	printf("do_gettimeofday %ld\n",tv.tv_sec);
	printf("do_gettimeofday micro second %ld\n",tv.tv_usec);
	
	return 0;
}

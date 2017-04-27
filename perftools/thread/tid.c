#include <stdio.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <pthread.h>


pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int  counter = 0;

pid_t gettid()
{
	return syscall( __NR_gettid );
}
void loopA()
{
	int i,j,k;
	for(i = 0; i < 100000000; i++)
		for(j = 0; j<5; j++)
			k=k;
}
void loopB()
{
	int i,j,k;
	for(i = 0; i < 100000000; i++)
		for(j = 0; j<5; j++)
			k=k;
}
void loopC()
{
	int i,j,k;
	for(i = 0; i < 100000000; i++)
		for(j = 0; j<5; j++)
			k=k;
}
void *mythreadA(void *data)
{
	int i;
	for( i = 0; i < 5; i++) {
//		pthread_mutex_lock( &mutex1 );
		counter++;
		loopA();
		printf("Counter %d. tid %d pid %d.\n", counter, gettid(), getpid());
//		pthread_mutex_unlock( &mutex1 );
	}
}
void *mythreadB(void *data)
{
	int i;
	for( i = 0; i < 5; i++) {
//		pthread_mutex_lock( &mutex1 );
		counter++;
		loopB();
		printf("Counter %d. tid %d pid %d.\n", counter, gettid(), getpid());
//		pthread_mutex_unlock( &mutex1 );
	}
}
void *mythreadC(void *data)
{
	int i;
	for( i = 0; i < 5; i++) {
//		pthread_mutex_lock( &mutex1 );
		counter++;
		loopC();
		printf("Counter %d. tid %d pid %d.\n", counter, gettid(), getpid());
//		pthread_mutex_unlock( &mutex1 );
	}
}
int main()
{
	pthread_t tid[3];
	pthread_create(&tid[0], NULL, mythreadA, NULL);
	pthread_create(&tid[1], NULL, mythreadB, NULL);
	pthread_create(&tid[2], NULL, mythreadC, NULL);
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
	pthread_join(tid[2], NULL);
	printf("all thread is finished. my tid is %d mypid is %d\n", gettid(), getpid());
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int  counter = 0;
int loop = 1000;

void *counter_func_1()
{
	int i;

	for (i=0; i < loop; i++) {
		pthread_mutex_lock( &mutex1);
		counter++;
		printf("thread 1 counter = %d\n",counter);
		pthread_mutex_unlock( &mutex1 );
	}
}

void *counter_func_2()
{
	int i;

	for (i=0; i < loop; i++) {
		pthread_mutex_lock( &mutex1);
		counter++;
		printf("thread 2 counter = %d\n",counter);
		pthread_mutex_unlock( &mutex1 );
	}
}


int main(int argc, char** argv)
{
	int rc1, rc2;
	pthread_t thread1, thread2;

	if((rc1=pthread_create( &thread1, NULL, &counter_func_1, NULL))) {
		printf("Thread creation failed: %d\n", rc1);
	}

	if( (rc2=pthread_create( &thread2, NULL, &counter_func_2, NULL))) {
		printf("Thread creation failed: %d\n", rc2);
	}

	pthread_join( thread1, NULL);
	pthread_join( thread2, NULL); 

	return 0;
}


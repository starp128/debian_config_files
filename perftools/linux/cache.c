#include <stdio.h>
#include <stdint.h>
#include <x86intrin.h>  // here: /usr/lib/gcc/x86_64-linux-gnu/10/include/emmintrin.h
#include <time.h>

int main()
{
	/* create array */
	int array[ 100 ];
	int i;
	for ( i = 0; i < 100; i++ )
		array[ i ] = i;   // bring array to the cache

	uint64_t t1, t2, ov, diff1, diff2, diff3;

	/* flush the first cache line */
	_mm_lfence();
	_mm_clflush( &array[ 30 ] );
	_mm_clflush( &array[ 70 ] );
	_mm_lfence();

	/* READ MISS 1 */
	_mm_lfence();           // fence to keep load order
	t1 = __rdtsc();         // set start time
	_mm_lfence();
	volatile int tmp = array[ 30 ];   // read the first elemet => cache miss
	_mm_lfence();
	t2 = __rdtsc();         // set stop time
	_mm_lfence();

	diff1 = t2 - t1;        // two fence statements are overhead

	//printf( "tmp is %d\ndiff1 is %lu\n", tmp, diff1 );

	/* READ MISS 2 */
	_mm_lfence();           // fence to keep load order
	t1 = __rdtsc();         // set start time
	_mm_lfence();
	tmp = array[ 70 ];      // read the second elemet => cache miss (or hit due to prefetching?!)
	_mm_lfence();
	t2 = __rdtsc();         // set stop time
	_mm_lfence();

	diff2 = t2 - t1;        // two fence statements are overhead
	//printf( "tmp is %d\ndiff2 is %lu\n", tmp, diff2 );


	/* READ HIT*/
	_mm_lfence();           // fence to keep load order
	t1 = __rdtsc();         // set start time
	_mm_lfence();

	//tmp = array[ 30 ];   // read the first elemet => cache hit

	(void) *((volatile int*)array + 30); //this one latency is lower than tmp = array[30]

	_mm_lfence();
	t2 = __rdtsc();         // set stop time
	_mm_lfence();

	diff3 = t2 - t1;        // two fence statements are overhead
	//printf( "tmp is %d\ndiff3 is %lu\n", tmp, diff3 );


	/* measuring fence overhead */
	_mm_lfence();
	t1 = __rdtsc();
	_mm_lfence();
	_mm_lfence();
	t2 = __rdtsc();
	_mm_lfence();
	ov = t2 - t1;

	printf( "lfence overhead is %lu\n", ov );
	printf( "cache miss1 TSC is %lu\n", diff1-ov );
	printf( "cache miss2 (or hit due to prefetching) TSC is %lu\n", diff2-ov );
	printf( "cache hit TSC is %lu\n", diff3-ov );


	return 0;
}

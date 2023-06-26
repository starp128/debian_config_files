/*
  struct iovec
  /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/uio.h>



void usage()
{
	printf("read a char from offset in file\n");
	printf("\tcommand file offset\n");
}

void test_readv(int fd, unsigned long offset)
{
	struct iovec iov[2];
	ssize_t nwritten;
	void *buf1;
	void *buf2;
	void *buf_notused;

	assert(posix_memalign(&buf1, 4096, 4096) == 0);
	assert(posix_memalign(&buf_notused, 4096, 4096) == 0);
	assert(posix_memalign(&buf2, 4096, 4096) == 0);

	iov[0].iov_base = buf1;
	iov[0].iov_len = 512;
	iov[1].iov_base = buf2;
	iov[1].iov_len = 512;

	nwritten = readv(fd, iov, 2);

	assert(nwritten == 1024);

	/*
	read_ret = read(fd, &result, 4);
	if( read_ret < 4)
	{
		printf("read error");
		return -1;
	}

	printf("read int from offset 0x%lx -> ", offset_in_file);
	printf("0x%08x\n", result);
	*/
	
}

int main(int argc, char** argv)
{
        int fd_in;
	unsigned int result = 0;
        int read_ret, ret;
	unsigned long offset_in_file = 0;


	if (argc != 3) {
		usage();
		return -1;
	}

	/* open file */
	fd_in = open(argv[1],O_RDONLY|O_DIRECT);
        if( fd_in == -1)
        {
                printf("open error\n");
		return -1;
        }

	/* seek to offset */
	offset_in_file = atoi(argv[2]);
	ret=lseek(fd_in, offset_in_file, SEEK_SET);
	if(ret==-1)
	{
		printf("lseek error\n");
		return -1;
	}


	test_readv(fd_in, offset_in_file);
	
	close(fd_in);
}


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<stdio.h>
#include<unistd.h>

void usage()
{
	printf("read a char from offset in file\n");
	printf("\tcommand file offset\n");
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
	fd_in = open(argv[1],O_RDONLY);
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

	/* read */
	read_ret = read(fd_in, &result, 4);
	if( read_ret < 4)
	{
		printf("read error");
		return -1;
	}

	printf("read int from offset 0x%lx -> ", offset_in_file);
	printf("0x%08x\n", result);

	close(fd_in);
}

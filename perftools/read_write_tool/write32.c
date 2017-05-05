#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<stdio.h>
#include<unistd.h>

void usage()
{
	printf("write int to offset in file\n");
	printf("\tcommand file offset value\n");
}

int main(int argc, char** argv)
{
        int fd_in;
	unsigned int result = 0;
        int read_ret, ret;
	unsigned long offset_in_file = 0;
	unsigned long value_to_write = 0;


	if (argc != 4) {
		usage();
		return -1;
	}

	/* open file */
	fd_in = open(argv[1],O_RDWR);
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

	/* val to write */
	value_to_write = atoi(argv[3]);

	/* write */
	ret = write(fd_in, &value_to_write, 4);
	if( ret < 4)
	{
		printf("write error");
		return -1;
	}

	printf("write int 0x%08x to offset 0x%lx\n", value_to_write, offset_in_file);

	close(fd_in);
}

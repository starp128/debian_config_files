#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define IOLEN 1

char *filename;
int offset;
unsigned int ch;

int main(int argc, char** argv)
{
	int fd, ret;
	char *endptr;

	if (argc != 3) {
		printf("%s filename offset\n");
		return -1;
	}

	filename = argv[1];
	offset = strtoul(argv[2], &endptr, 0);

        fd = open(filename, O_RDONLY);
        if (fd == -1) {
		perror("open error\n");
		return -1;
        }

        ret = pread(fd, &ch, IOLEN, offset);
	if (ret != IOLEN) {
		printf("read fialed\n");
		close(fd);
		return -1;
	}

	printf("offset 0x%x: 0x%x\n", offset, ch);

	close(fd);
        return 0;
}

#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define IOLEN 4

unsigned long ch;
char *filename;
unsigned long offset;

int main(int argc, char** argv)
{
	int fd, ret;
	char *endptr;

	if (argc != 4) {
		printf("%s filename offset char\n");
		return -1;
	}

	filename = argv[1];
	offset = strtoul(argv[2], &endptr, 0);
	ch = strtoul(argv[3], &endptr, 0);

        fd = open(filename, O_RDWR, 0777);
        if (fd == -1) {
		printf("open error\n");
		return -1;
        }

        ret = pwrite(fd, &ch, IOLEN, offset);
	if (ret != IOLEN) {
		printf("write fialed\n");
		close(fd);
		return -1;
	}

	close(fd);
        return 0;
}

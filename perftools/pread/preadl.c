#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char** argv)
{
	int fd, ret;
	unsigned int ch;
	char *filename;
	int offset;
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

        ret = pread(fd, &ch, 4, offset);
	if (ret != 4) {
		printf("read fialed\n");
		close(fd);
		return -1;
	}

	printf("offset 0x%x: 0x%x\n", offset, ch);

	close(fd);
        return 0;
}

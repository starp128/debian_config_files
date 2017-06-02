#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char** argv)
{
	int fd, ret;
	char ch;
	char *filename;
	int offset;

	if (argc != 3) {
		printf("%s filename offset\n");
		return -1;
	}

	filename = argv[1];
	offset = atoi(argv[2]);

        fd = open(argv[1], O_RDONLY);
        if (fd == -1) {
		perror("open error\n");
        }

        ret = pread(fd, &ch, 1, offset);
	if (ret != 1) {
		printf("read fialed\n");
		close(fd);
		return -1;
	}

	printf("offset 0x%x: 0x%x\n", offset, ch);

	close(fd);
        return 0;
}

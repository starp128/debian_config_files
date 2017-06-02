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

	if (argc != 4) {
		printf("%s filename offset char\n");
		return -1;
	}

	filename = argv[1];
	offset = atoi(argv[2]);
	ch = atoi(argv[3]);

        fd = open(filename, O_RDWR, 0777);
        if (fd == -1) {
		printf("open error\n");
		return -1;
        }

        ret = pwrite(fd, &ch, 1, offset);
	if (ret != 1) {
		printf("write fialed\n");
		close(fd);
		return -1;
	}

	close(fd);
        return 0;
}

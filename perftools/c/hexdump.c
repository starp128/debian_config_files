#include <stdio.h>


static void dump_data(void *data, int len, char *message)
{
	int i;
	int line, remain, num;

	line = len/32;
	remain = len%32;

	printf("%s\n", message);

	for (i = 0; i < line ; i++) {
		printf("[0x%016lx]: 0x%016lx 0x%016lx 0x%016lx 0x%016lx\n",
		       (unsigned long)data,
		       *(unsigned long*)data,
		       *(unsigned long*)(data + 8),
		       *(unsigned long*)(data + 16),
		       *(unsigned long*)(data + 24));
		data += 32;
	}

	num = remain/8;

	switch (num) {
	case 1:
		printf("[0x%016lx] 0x%016lx\n",
		       (unsigned long)data,
		       *(unsigned long*)data);
		break;
	case 2:
		printf("[0x%016lx] 0x%016lx 0x%016lx\n",
		       (unsigned long)data,
		       *(unsigned long*)data,
		       *(unsigned long*)(data + 8));

		break;
	case 3:
		printf("[0x%016lx] 0x%016lx 0x%016lx 0x%016lx\n",
		       (unsigned long)data,
		       *(unsigned long*)(data +8),
		       *(unsigned long*)(data +16),
		       *(unsigned long*)(data +24));
		break;
	}
}

int main(int argc, char** argv)
{
	char buf[128];
	int i;

	for (i = 0; i < 128; i++) {
		buf[i] = i;
	}

	dump_data(buf, 3, "aa");
	dump_data(buf, 4, "bb");

	dump_data(buf, 5, "cc");
	dump_data(buf, 8, "dd");
	dump_data(buf, 16, "dd16");

	dump_data(buf, 32, "dd");
	dump_data(buf, 64, "dd");
	dump_data(buf, 72, "dd");

	return 0;
}

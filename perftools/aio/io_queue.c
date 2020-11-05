/*
 * gcc io_test.c -D_GNU_SOURCE -laio -o io_test -g -O0
 */
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <stdint.h>
#include <getopt.h>
#include <libaio.h>


static uint64_t rdtsc(void)
{
        unsigned int lo,hi;
        __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
        return ((uint64_t)hi << 32) | lo;
}

#define dbg(fmt, args...) do { if (prog_opt.debug) printf(fmt, ##args); } while (0)
#define err(fmt, args...) do { fprintf(stderr, ""fmt, ##args); exit(-1);} while (0)
#define info(fmt, args...) do { printf(fmt, ##args); } while (0)

#define MAX_DEPTH 1024

struct ios {
	int index;
	void *buf;
	struct iocb iocb;
};
struct ios ios[MAX_DEPTH];

struct prog_opt {
	int debug;
	int write;
	int bs;
	int count;
	int depth;
	char *file;
} prog_opt;

static struct option long_options[] = {
        {"help",                no_argument,          NULL,        'h'},
        {"debug",               no_argument,          NULL,        'd'},
        {"bs",                  required_argument,       NULL,      'b'},
        {"count",               required_argument,       NULL,       'c'},
	{"depth",               required_argument,       NULL,       'D'},
	{"write",               no_argument,		NULL,       'w'},
	{"file",		required_argument,	NULL,	    'f'},
};

void opt_help()
{
	printf("--write\n");
	printf("--bs=4k\n");
	printf("--count=1\n");
	printf("--depth=1\n");
	printf("--file=/dev/sdb1\n");
}

void opt_init()
{
	memset(&prog_opt, 0, sizeof(prog_opt));
	prog_opt.bs = 4096;
	prog_opt.count = 1;
	prog_opt.depth = 1;
}

void opt_parse(int argc, char *argv[])
{
        int c, option_index;

        while ((c = getopt_long(argc, argv, "hdb:c:d:", long_options,
                        &option_index)) != -1) {
                switch (c) {
                case 'h':
                        opt_help();
                        break;
                case 'd':
                        prog_opt.debug = 1;
                        break;
		case 'b':
			prog_opt.bs = atoi(optarg);
			break;
		case 'c':
			prog_opt.count = atoi(optarg);
			break;
		case 'D':
			prog_opt.depth = atoi(optarg);
			break;
		case 'w':
			prog_opt.write = 1;
			break;
		case 'f':
			prog_opt.file = strdup(optarg);
			break;
		default:
			printf("unknow opcode\n");
			break;
		}
	}

	if (prog_opt.file == NULL)
		err("specify --file\n");

}


void init_ios()
{
	int i;
	int err;

	for (i = 0; i < prog_opt.depth ; i++) {
		ios[i].index = i;
		err = posix_memalign((void**)&ios[i].buf, sysconf(_SC_PAGESIZE), prog_opt.bs);
		if (err) {
			printf("err\n");
			exit(1);
		}
		strcpy(ios[i].buf, "hello xxx");
	}
}

int main(int argc, char *argv[])
{
	io_context_t ctx;
	unsigned long i;
	int ret;
	struct io_event events[MAX_DEPTH];
	struct iocb *iocbpp[MAX_DEPTH] = {0};
	int fd;

	opt_init();
	opt_parse(argc, argv);

	fd = open(prog_opt.file, O_CREAT|O_DIRECT|O_RDWR, S_IRWXU|S_IRWXG|S_IROTH);
	if (fd < 0 ) {
		printf("open: %s\n", strerror(errno));
		err("could not open\n");
	}

	memset(&ctx, 0, sizeof(ctx));
	ret = io_setup(MAX_DEPTH, &ctx);
	if (ret)
		err("io_setup err\n");

	init_ios();
	memset(iocbpp, 0, sizeof(struct iocb *) * MAX_DEPTH);

	for (i = 0; i < prog_opt.depth; i++) {
		ios[i].iocb.data           = (void *)i;
		if (prog_opt.write)
			ios[i].iocb.aio_lio_opcode = IO_CMD_PWRITE;
		else
			ios[i].iocb.aio_lio_opcode = IO_CMD_PREAD;
		ios[i].iocb.aio_reqprio    = 0;
		ios[i].iocb.aio_fildes     = fd;

		ios[i].iocb.u.c.buf    = ios[i].buf;
		ios[i].iocb.u.c.nbytes = prog_opt.bs;
		ios[i].iocb.u.c.offset = prog_opt.bs * i;

		iocbpp[i] = &ios[i].iocb;
	}

	int n;
	int done = 0;
	uint64_t tsc_start = rdtsc();
	n = io_submit(ctx, prog_opt.depth, iocbpp);
	if (n < 0)
		perror("io_sbumit\n");

	while(1) {
		int m;
		n = io_getevents(ctx, 1, prog_opt.depth, events, NULL);
		if (n < 0) {
			printf("error getevents\n");
			close(fd);
			io_destroy(ctx);
		} else if (n == 0)
			continue;

		done += n;
		info("total %d this round %d completion\n", done, n);

		for (m = 0; m < n; m ++) {
			unsigned long index;
			struct iocb *piocb;

			
			index = (unsigned long)events[m].data;
			piocb = &ios[index].iocb;
			io_submit(ctx, 1, &piocb);

			dbg("resubmit %ld\n", index);

		}

		if (done >= prog_opt.count) {
			break;
		}
	}

	close(fd);
	io_destroy(ctx);

	return 0;
}

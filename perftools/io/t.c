#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<stdio.h>
#include<unistd.h>

int arg_skip=0;
char* version="0.1";

void usage()
{
	printf("tools to parse perf.data");
	printf("command [option] file\n");
	printf("option can be used:\n");
	printf("\t-h\n\t\tshow this message\n");
	printf("\t-v\n\t\tshow version\n");
	printf("\t-s num\n\t\tskip num's byte when read\n");
}
void show_version()
{
	printf("%s\n",version);
}
int main(int argc, char** argv)
{
        int fd_in;
        char buf[50];
        char ch;
        int read_ret;
        int i;
        int done=0;
        int total_read=0;
	unsigned int *p;
	int option;
	int ret;

	while((option = getopt(argc,argv,"hs:v"))!=-1)
	{
		switch(option)
		{
		case 'h':
			usage();
			return 0;
		case 's':
			//printf("%s\n",optarg);
			//printf("%lx\n",optarg);
			//printf("%x\n",&optarg);
			arg_skip = atoi(optarg);
			break;
		case 'v':
			show_version();
			return 0;
		default:
			printf("unknow option %c",option);
			return 1;
		}
	}

	printf("%s\n",argv[optind]);
	fd_in = open(argv[optind],O_RDONLY);
        if( fd_in == -1)
        {
                printf("open error\n");
        }

	ret=lseek(fd_in, arg_skip, SEEK_SET);
	if(ret==-1)
	{
		printf("lseek error\n");
	}
        while(!done)
        {
                read_ret = read(fd_in, buf, 24);
                if( read_ret < 0)
                {
                        printf("read error");
                        break;
                }
                else if (read_ret == 0)
                {
                        printf("read finished\n");
                        done=1;
                }
                else
                {
			if(read_ret!=24)
				break;
                        total_read+=read_ret;
                        for(i=15;i>7;i--)
                        {
                                ch = buf[i];
                                printf("%02x",(unsigned char)ch);
                        }
			printf(" ");
			p=(unsigned int*)&buf[16];
			printf("pid=%d ",*p);
			p=(unsigned int*)&buf[20];
			printf("tid=%d",*p);
                        printf("\n");
                }
        }
	close(fd_in);
}

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#define SCSI_LOG_ERROR_SHIFT              0
#define SCSI_LOG_TIMEOUT_SHIFT            3
#define SCSI_LOG_SCAN_SHIFT               6
#define SCSI_LOG_MLQUEUE_SHIFT            9
#define SCSI_LOG_MLCOMPLETE_SHIFT         12
#define SCSI_LOG_LLQUEUE_SHIFT            15
#define SCSI_LOG_LLCOMPLETE_SHIFT         18
#define SCSI_LOG_HLQUEUE_SHIFT            21
#define SCSI_LOG_HLCOMPLETE_SHIFT         24
#define SCSI_LOG_IOCTL_SHIFT              27

#define SCSI_LOG_ERROR_BITS               3
#define SCSI_LOG_TIMEOUT_BITS             3
#define SCSI_LOG_SCAN_BITS                3
#define SCSI_LOG_MLQUEUE_BITS             3
#define SCSI_LOG_MLCOMPLETE_BITS          3
#define SCSI_LOG_LLQUEUE_BITS             3
#define SCSI_LOG_LLCOMPLETE_BITS          3
#define SCSI_LOG_HLQUEUE_BITS             3
#define SCSI_LOG_HLCOMPLETE_BITS          3
#define SCSI_LOG_IOCTL_BITS               3

struct log_group {
	char *name;
	int shift;
} log_group[] = {
	{"error",              0},
	{"timeout",            3},
	{"scan",               6},
	{"mlqueue",            9},
	{"mlcomplete",         12},
	{"llqueue",            15},
	{"llcomplete",         18},
	{"hlqueue",            21},
	{"hlcomplete",         24},
	{"ioctl",              27},
	{NULL,                 0}
};


void list()
{
	int i = 0;

	while(log_group[i].name) {
		printf("%s\n", log_group[i].name);
		i++;
	}
}

struct log_group * name_to_group(char *group)
{
	int i = 0;

	while(log_group[i].name) {
		if (!strcmp(log_group[i].name, group))
			return &log_group[i];
		i++;
	}

	return NULL;
}


void get(char *group)
{
	struct log_group *log = name_to_group(group);

	if (log == NULL) {
		printf("could not found %s\n", group);
		return;
	}

	int shift;
	shift = log->shift;
	printf("cat /proc/sys/dev/scsi/logging_level & %x\n", 1 << shift);
}

void set(char *group, int value)
{
	struct log_group *log = name_to_group(group);

	if (log == NULL) {
		printf("could not found %s\n", group);
		return;
	}

	int shift;
	shift = log->shift;
	printf("echo 0x%x > /proc/sys/dev/scsi/logging_level\n", value << shift);
}



void help()
{
	printf("This tool helps setting scsi logglevel\n");
	printf(" -h --help       show this message\n");
	printf(" -l --list       list avaiable logging levels\n");
	printf(" -s --set        setup log group\n");
	printf(" -g --get        get current log group level\n");
	printf(" -c --clear-all  clear all log group logging level\n");
}

static struct option long_options[] = {
        {"help",           no_argument,          NULL,        'h'},
        {"list",           no_argument,          NULL,        'l'},
        {"get",            required_argument,    NULL,        'g'},
        {"set",            required_argument,    NULL,        's'},
	{"value",          required_argument,    NULL,        'v'},
	{"clear-all",      no_argument,          NULL,        'c'},
        {0, 0, 0, 0}
};

int main(int argc, char *argv[])
{
        int c, option_index;
	int get_mode = 0, set_mode = 0;
	char *group = NULL;
	int value = 0;

        while ((c = getopt_long(argc, argv, "h?lg:s:v:c", long_options,
                        &option_index)) != -1) {
                switch (c) {
                case 'h':
                case '?':
                        help();
                        break;
                case 'l':
			list();
                        exit(0);
		case 'c':
			//clear_all();
			break;
		case 'g':
			get_mode = 1;
			group = strdup(optarg);
			break;
		case 's':
			set_mode = 1;
			group = strdup(optarg);
			break;
		case 'v':
			value = atoi(optarg);
			break;
		default:
			help();
			exit(1);
		}
	}

	if (get_mode == 0 && set_mode == 0) {
		help();
		exit(1);
	}
	if (get_mode) {
		get(group);
	} else if (set_mode) {
		set(group, value);
	}

	return 0;
}

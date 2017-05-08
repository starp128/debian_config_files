/* implement a internal buffer and function for binary file dump
 * 
 * Usage: bin_printf(1, "xxx\n");
 *        bin_printf(0, "add\n");
 *        bin_printf(0, "add\n");
 *        ... repeat
 *        binary_dump_with_string(off, size, buf, bin_buffer());

 * Or   : bin_buffer_reset()
 *        bin_printf(0, "what you want\n")
 *        ... repeat
 *        binary_dump_with_string(off, size, buf, bin_buffer());

 * Design: not depend on other function or variable except libc
           export API no global variable
	   in  --API are prefix with "bin"
	   out --the output is fixed to stderr
	   error handling --no

 */

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdarg.h>
#include "comm.h"

#define MAX 4096

static char screen[MAX];

static unsigned int buf_index;

char* bin_buffer()
{
	return screen; 		/* good for library release */
}

void bin_buffer_reset()
{
	buf_index = 0;
}
int bin_printf(int first, char* fmt, ...)
{
	int number;
	va_list args;
	if (first) {
		buf_index = 0;
	}
	if (!fmt || *fmt == 0) {
		return buf_index;
	}
	if (buf_index > MAX || buf_index < 0) {
		perr("screen buffer overflow");
		return -1;
	}
	va_start(args, fmt);
	number = vsprintf(screen + buf_index, fmt, args);
	va_end(args);
	buf_index += number;
	return buf_index; /* return total writed number in screen buffer */
}



void binary_dump(unsigned int offset, unsigned int size, unsigned char* buf)
{
	int i, j, k;
	int gap = 0;
	unsigned char* p = buf;

	if (!size || buf == NULL)
	{
		return;
	}

	gap = offset % 16;
	offset = offset / 16 * 16;

	/* begin */
	fprintf(stderr, "%08x  ", offset); /* print the offset in file */
	for (i = 0; i < gap; i++)
	{
		fprintf(stderr, "   "); /* out the gap */
	}

	for (j = gap; j < gap + size; j++)
	{
		if ((j + 1) % 16 == 0) {
			fprintf(stderr, "%02x", *(p + j -gap));
			if ( j + 1 != gap + size) {
				fprintf(stderr, "\n%08x  ", offset += 16);
			}
		} else {
			fprintf(stderr, "%02x ", *(p + j -gap));
		}
	}
	fprintf(stderr, "\n\n");
	/* Maybe write to a big buffer instead of the stderr.
	   So we can goto x,y to the screen */
}

static unsigned char printable(unsigned char c)
{
	if ('a' <= c && c <= 'z')
		return c;
	if ('A' <= c && c <= 'Z')
		return c;
	if ('0' <= c && c <= '9')
		return c;
	if (c == 0) return '@';
	return '.';
}

void binary_dump_with_char(unsigned int offset, unsigned int size,
			  unsigned char* buf)
{
	int i, j, k;
	int gap = 0;
	unsigned char* p = buf;
	int line_index = 0;
	int post_gap;
	
	unsigned char tailer[17];

	if (!size || buf == NULL)
	{
		return;
	}

	memset((void*)tailer, ' ', 16);
	tailer[16] = 0;

	gap = offset % 16;
	offset = offset / 16 * 16;

	/* pre gap */
	fprintf(stderr, "%08x  ", offset); /* print the offset in file */
	for (i = 0; i < gap; i++)
	{
		fprintf(stderr, "   "); /* out the gap */
	}

	for (j = gap; j < gap + size; j++)
	{
		if ((j + 1) % 16 == 0) {
			fprintf(stderr, "%02x | ", *(p + j -gap));
			tailer[15] = printable(*(p + j -gap));
			fprintf(stderr, "%s", (char*)tailer);
			if ( j + 1 != gap + size) {
				fprintf(stderr, "\n%08x  ", offset += 16);
				memset(tailer, ' ', 16);
				tailer[16] = 0;
			}
			line_index++;
		} else {
			fprintf(stderr, "%02x ", *(p + j -gap));
			tailer[j%16] = printable(*(p + j -gap));
		}
	}
	
	/* post gap */
	post_gap = j % 16; 	/* post_gap == 0 measn no need to add tailer */
	if (post_gap) {
		for (k = post_gap; k< 16; k++)
		{
			if (k + 1 == 16) {
				fprintf(stderr, "   | ");
			} else {
				fprintf(stderr, "   ");
			}
		}
		fprintf(stderr, "%s", (char*)tailer);
	}
	fprintf(stderr, "\n\n");
}


void binary_dump_with_comment(unsigned int offset, unsigned int size,
			     unsigned char* buf, int argc, char** argv)
{
	int i, j, k;
	int gap = 0;
	unsigned char* p = buf;
	int line_index = 0;
	int post_gap;

	if (!size || buf == NULL)
	{
		return;
	}
	gap = offset % 16;
	offset = offset / 16 * 16;

	/* pre gap */
	fprintf(stderr, "%08x  ", offset); /* print the offset in file */
	for (i = 0; i < gap; i++)
	{
		fprintf(stderr, "   "); /* out the gap */
	}

	for (j = gap; j < gap + size; j++)
	{
		if ((j + 1) % 16 == 0) {
			fprintf(stderr, "%02x | ", *(p + j -gap));

			if (line_index < argc) {
				fprintf(stderr, argv[line_index]);
			} 
			if ( j + 1 != gap + size) {
				fprintf(stderr, "\n%08x  ", offset += 16);
			}
			line_index++;
		} else {
			fprintf(stderr, "%02x ", *(p + j -gap));
		}
	}
	
	/* post gap */
	post_gap = j % 16;
	if (post_gap) {
		for (k = post_gap; k< 16; k++)
		{
			if (k + 1 == post_gap) {
				fprintf(stderr, "   | ");
				if (line_index < argc) {
					fprintf(stderr, argv[line_index++]);
				}
			} else {
				fprintf(stderr, "   ");
			}
		}
	}
	fprintf(stderr, "\n");

	/* if still exist comment */
	for (; line_index < argc; line_index++)
	{
		fprintf(stderr,
			"                                                            %s\n",
			argv[line_index]);
	}
	fprintf(stderr, "\n");
}


/* trailer is a string add to the raw data.
 * is line of buf is short than the that of trailer, space will be add to the buf
 */
void binary_dump_with_string(unsigned int offset, unsigned int size,
			     unsigned char* buf, char* str)
{
	int i, j, k;
	int gap = 0;
	unsigned char* p = buf;
	int line_index = 0;
	int post_gap;
	char* tail;
	char* head;
	int done = 0;
	head = tail = str;

	if (!size || buf == NULL)
	{
		return;
	}
	gap = offset % 16;
	offset = offset / 16 * 16;

	/* pre gap */
	fprintf(stderr, "%08x  ", offset); /* print the offset in file */
	for (i = 0; i < gap; i++)
	{
		fprintf(stderr, "   "); /* out the gap */
	}

	for (j = gap; j < gap + size; j++)
	{
		if ((j + 1) % 16 == 0) {
			fprintf(stderr, "%02x | ", *(p + j -gap));
			if (head && *head) {
				tail = strchr(tail, '\n');
				if (tail && *tail) {
					/* not end of the str*/
					*tail = '\0'; /* mark the end of this line */
					tail++;	     /* move to the next head */
				}
				fprintf(stderr, "%s", head); /* add the tail string */
				head = tail;
			}
			
			if ( j + 1 != gap + size) {
				fprintf(stderr, "\n%08x  ", offset += 16);
			}
			line_index++;
		} else {
			fprintf(stderr, "%02x ", *(p + j -gap));
		}
	}
	
	/* post gap */
	if (head && *head) {
		post_gap = j % 16;
		if (post_gap) {
			for (k = post_gap; k< 16; k++)
			{
				if (k + 1 == 16) {
					fprintf(stderr, "   | ");
				} else {
					fprintf(stderr, "   ");
				}
			}
		}
		tail = strchr(tail, '\n');
		if (tail && *tail) {
			/* not end of the str*/
			*tail = '\0'; /* mark the end of this line */
			tail++;	     /* move to the next head */
		}
		fprintf(stderr, "%s\n", head); /* add the tail string */
		head = tail;
	}

	/* If there are still str */
	while (head && *head) {
		tail = strchr(tail, '\n');
		if (tail && *tail) {
			/* not end of the str*/
			*tail = '\0'; /* mark the end of this line */
			tail++;	     /* move to the next head */
		}
		fprintf(stderr,
			"                                                            %s\n",
			head);
		head = tail;
	}


	fprintf(stderr, "\n");
}

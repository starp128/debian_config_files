#ifndef _BINARY_DUMP_H
#define _BINARY_DUMP_H

extern void binary_dump_with_string(unsigned int offset, unsigned int size,
			     unsigned char* buf, char* str);
extern void binary_dump_with_comment(unsigned int offset, unsigned int size,
			      unsigned char* buf, int argc, char** argv);
extern void binary_dump_with_char(unsigned int offset, unsigned int size,
				  unsigned char* buf);
extern void binary_dump(unsigned int offset, unsigned int size, unsigned char* buf);

extern int bin_printf(int first, char* fmt, ...);

extern char* bin_buffer();

extern void bin_buffer_reset();

#endif/* _BINARY_DUMP_H */

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>

typedef int (*_get_number_line)(const char* filename,void** p);



void main(){
	FILE* fp;
	unsigned char fmem[1024];
	int memLen;
	void* p[6];
	_get_number_line get_number_line;
	void* function;
	void* fun2;
	int fd;
	int fd2;

	p[0]=stat;
	p[1]=fopen;
	p[2]=malloc;
	p[3]=fread;
	p[4]=fclose;
	p[5]="rb";

	fp=fopen("./get_line_number.bin","rb");
	memLen=fread(fmem,1,1024,fp);
	fclose(fp);

	fd=open("./get_line_number.bin",O_RDONLY);
	function=mmap(NULL,memLen,PROT_EXEC|PROT_READ|PROT_WRITE,MAP_PRIVATE,fd,0);

	get_number_line=(_get_number_line) function;
	if(get_number_line==NULL){
		printf("install function fail.");
		return;
	}
	printf("%p\n",get_number_line);
	printf("total line:%d\n",get_number_line("./test.txt",p));

	munmap(get_number_line,memLen);
	close(fd);


	fd2=open("./another.bin",O_RDONLY);
	fun2=mmap(function,memLen,PROT_EXEC|PROT_READ|PROT_WRITE,MAP_PRIVATE,fd2,0);
	get_number_line=(_get_number_line) fun2;
	if(get_number_line==NULL){
		printf("install function fail.");
		return;
	}
	printf("%p\n",get_number_line);
	printf("two total line:%d\n",get_number_line("./test.txt",p));
	munmap(get_number_line,memLen);
	close(fd2);
}

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>


#include <sys/mman.h>

typedef int (*_get_number_line)(const char* filename,void** p);



void* install_function(void* mem,int len){
	void* function;
	function=mmap(NULL,len,PROT_EXEC|PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
	if(function==-1) return NULL;
	memcpy(function,mem,len);
	return function;
}



void main(){
	FILE* fp;
	unsigned char fmem[1024];
	int memLen;
	void* p[6];
	_get_number_line get_number_line;

	p[0]=stat;
	p[1]=fopen;
	p[2]=malloc;
	p[3]=fread;
	p[4]=fclose;
	p[5]="rb";

	fp=fopen("./get_line_number.bin","rb");
	memLen=fread(fmem,1,1024,fp);
	fclose(fp);

	get_number_line=(_get_number_line) install_function(fmem,memLen);
	if(get_number_line==NULL){
		printf("install function fail.");
		return;
	}

	printf("total line:%d\n",get_number_line("./test.txt",p));

	munmap(get_number_line,memLen);
	
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

typedef int (* __stat)( const char *path, struct stat *buffer );
typedef FILE* (* __fopen)( const char *filename, const char *mode );
typedef void * (* __malloc)( size_t size );
typedef size_t (* __fread)( void *buffer, size_t size, size_t count, FILE *stream );
typedef int (* __fclose)( FILE *stream );

int get_number_line(const char* filename,void **p){
	FILE* fp;
	char* buffer;
	struct stat st;
	int num=0;
	int i;
	__stat _stat;
	__fread _fread;
	__fopen _fopen;
	__malloc _malloc;
	__fclose _fclose;
	const char* fmode;
	
	_stat=(__stat)p[0];
	_fopen=(__fopen)p[1];
	_malloc=(__malloc)p[2];
	_fread=(__fread)p[3];
	_fclose=(__fclose)p[4];
	fmode=(const char*)p[5];

	fp=_fopen(filename,fmode);
	if(fp==NULL) return -1;
	_stat(filename,&st);
	buffer=_malloc(st.st_size);
	if(buffer==NULL) return -1;
	_fread(buffer,st.st_size,1,fp);
	for(i=0;i<st.st_size;i++){
		if(buffer[i]=='\n') num++;
	}
	_fclose(fp);
	int m, n;
	for(m=0;m<1000000;m++)
	  for(n=0;n<5000;n++)
	    i=i;
	return num;
}

void main(int argc,char* argv[]){
	void* p[6];
	int i;
	p[0]=stat;
	p[1]=fopen;
	p[2]=malloc;
	p[3]=fread;
	p[4]=fclose;
	p[5]="rb";
	i=get_number_line("./test.txt",p);
	printf("%x  %d\n",get_number_line,i);
}

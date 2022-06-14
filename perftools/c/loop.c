#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

/* num between 1~ 
   ex 1000
*/
void fun_A(int num)
{
        int i,j,k,m,n;
        for (i=0;i<num;i++)
                for(j=0;j<1000;j++)
                        for(k=0;k<1000;k++)
                                for(n=0;n<25;n++)
                                        m=m;
}
void fun_B(int num)
{
        int i,j,k,m,n;
        for (i=0;i<num;i++)
                for(j=0;j<1000;j++)
                        for(k=0;k<1000;k++)
                                for(n=0;n<25;n++)
                                        m=m;
}
void fun_C(int num)
{
        int i,j,k,m,n;
        for (i=0;i<num;i++)
		for(k=0;k<1000;k++)
			for(n=0;n<25;n++)
				m=m;
	fun_A(500);
	fun_B(500);
}

int main()
{
	fun_C(500);
        printf("done looper\n");
	printf("my pid is %d\n",getpid());
        return 0;
}

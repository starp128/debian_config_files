#include<stdlib.h>
#include<stdio.h>

int main()
{
	int master, slave;
	char *name;
	char buf_name[1024];
	int ret;
	char buf[1024];

	master = getpt();
	if(master <0)
		return 1;
	printf("%d\n",master);
	if(grantpt(master) < 0||unlockpt(master)<0)
		return 2;
	
	if(ptsname_r(master, buf_name, 1024)!=0)
		return 4;
	printf("%s\n",buf_name);

	while(1)
	{
		ret = read(0, buf, 1024);
//		printf("ret = %d %s\n",ret, buf);
		
		write(master, buf, ret);
	}
	return 0;
}

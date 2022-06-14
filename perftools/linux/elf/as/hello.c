#include <stdio.h>

int global_var1 = 18;
int global_var2 = 19;

void fun(int a)
{
	printf("the param is %d\n", a);
}

int main(int argc, char** argv)
{
	fun(10);
	printf("global_var1 + global_var2 = %d", global_var1 + global_var2);
}

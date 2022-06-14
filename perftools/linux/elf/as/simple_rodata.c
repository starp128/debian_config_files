int other(const char* format, ...);

int global_init_var1 = 84;
int global_init_var2 = 11;
int global_init_var3 = 22222222;

int global_uninit_var;

void func1(int i)
{
	int a;
	int b;
	a = i;
	b = i;
	other("%d\n", a+b);
	other("hello world\n", a+b);
	other("nihao\n", a+b);
	other("%016llx\n", a+b);
}

int main(void)
{
	static int static_var1 = 85;
	static int static_var2 = 85;
	static int static_var3 = 85;
	static int static_var4 ;

	int a = 1;
	int b = 2;
	int c;
	a++;
	b++;
	c = a + b;
	c = c + static_var1;
	c = c + static_var2;
	c = c + static_var3;

	func1(c);

	return a;
}

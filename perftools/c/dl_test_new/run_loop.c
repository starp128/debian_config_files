void sub_a(int m, int n)
{
	int i,j,k=1;
	for(i=0;i<m;i++)
		for(j=0;j<n;j++)
			k=k;
}
void sub_b(int m, int n)
{
	int i,j,k=1;
	for(i=0;i<m;i++)
		for(j=0;j<n;j++)
			k=k;
}
void run_loop(int m, int n)
{
	int i,j,k=1;
	for(i=0;i<m;i++)
		for(j=0;j<n;j++)
			k=k;
	sub_a(m/2, n/2);
	sub_b(m/2, n/2);
}
void run_another_loop(int m, int n)
{
	int i,j,k=1;
	for(i=0;i<m;i++)
		for(j=0;j<n;j++)
			k=k;
}

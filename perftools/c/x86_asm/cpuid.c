#include <stdio.h>

/* 

1b 756e6547 6c65746e 49656e69
GenuntelineI

 */
void cpuid_eax0()
{
	int var1, var2, var3, var4;
	int op = 0;

	var1 = 0;

	asm ("cpuid"
	     : "=a" (var1),
	     "=b" (var2),
	     "=c" (var3),
	     "=d" (var4)
	     : "a" (op));

	//printf("eax=%x\n", var1);
	printf("%x %x %x %x\n", var1, var2, var3, var4);

	int i;
	char *p;
	p = &var2;
	for (i = 0 ; i < 4; i++) {
		printf("%c", *p);
		p++;
	}

	p = &var3;
	for (i = 0 ; i < 4; i++) {
		printf("%c", *p);
		p++;
	}

	p = &var4;
	for (i = 0 ; i < 4; i++) {
		printf("%c", *p);
		p++;
	}

	printf("\n");
	return;
}

/*
Stepping ID is a product revision number assigned due to fixed errata or other changes.
The actual processor model is derived from the Model, Extended Model ID and Family ID fields. If the Family ID field is either 6 or 15, the model is equal to the sum of the Extended Model ID field shifted left by 4 bits and the Model field. Otherwise, the model is equal to the value of the Model field.
The actual processor family is derived from the Family ID and Extended Family ID fields. If the Family ID field is equal to 15, the family is equal to the sum of the Extended Family ID and the Family ID fields. Otherwise, the family is equal to the value of the Family ID field.
The meaning of the Processor Type field is given in the table below.
 */
void cpuid_eax1()
{
	int var1, var2, var3, var4;
	int op = 1;

	asm ("cpuid"
	     : "=a" (var1),
	     "=b" (var2),
	     "=c" (var3),
	     "=d" (var4)
	     : "a" (op));

	printf("input: eax=%x\n", op);
	printf("output: %x %x %x %x\n", var1, var2, var3, var4);

	printf("ext model: 0x%x\n", (var1 >> 16) & 0xf);
	printf("model: 0x%x\n", (var1 & 0xf0) >> 4);
	printf("ext_model << 4 | model = %d \n", (((var1 >> 16) & 0xf) << 4) + ((var1 & 0xf0) >> 4));
	printf("family: 0x%x\n", (var1 & 0xf00) >> 8);
	printf("stepping: 0x%x\n", (var1 & 0xf));
	return;
}



int main()
{
	cpuid_eax0();
	cpuid_eax1();
	return 0;
}

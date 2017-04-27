#include <stdio.h>
#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdlib.h>

void get8(char* cp, char* src)
{
        int i,j,t;

        for(i=0;i<8;i++){
		t = src[i];
                for(j=0; j<8 ; j++) {
			*cp++ = (t & (0x01 << (7-j))) != 0;
                }
        }
}

void put8(char* cp, char* dst)
{
        int i,j,t;

        for(i=0;i<8;i++){
		t = 0;
		for(j = 0; j<8; j++)
			t = (t<<1) | *cp++;
		*dst = t;
		dst++;
        }
}


int
main(void)
{
	char key[64];      /* bit pattern for key */
	char txt[64];      /* bit pattern for messages */
	int i;
	char plain[8] = "hello";
	char res[8];
	for (i = 0; i < 64; i+=2) {
		key[i] = 1;
		key[i+1] = 0;
	}
	get8(txt, plain);

	setkey(key);
	encrypt(txt, 0);   /* encode */
	encrypt(txt, 1);   /* decode */

	put8(txt, res);
}

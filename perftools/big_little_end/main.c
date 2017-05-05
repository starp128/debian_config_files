/*
run on x86
$./a.out
buf[0] addr 0x1096c2018 11
buf[1] addr 0x1096c2019 22
buf[2] addr 0x1096c201a 33
addr 0x1096c2019 as short-> 3322
It is little end

run on s390
$./a.out 
buf[0] addr 0x1000203c 11
buf[1] addr 0x1000203d 22
buf[2] addr 0x1000203e 33
addr 0x1000203d as short-> 2233
It is big end
*/

#include <stdio.h>

char buf[3] = {0x11, 0x22, 0x33};

int main(int argc, char** argv)
{
        unsigned short *p;

        printf("buf[0] addr %p %02x\n", &buf[0], buf[0]);
        printf("buf[1] addr %p %02x\n", &buf[1], buf[1]);
        printf("buf[2] addr %p %02x\n", &buf[2], buf[2]);

        p = (unsigned short *)&buf[1];

        printf("addr %p as short-> %04x\n", p, *p);

        if (*p == 0x3322)
                printf("It is little end\n");
        else if (*p == 0x2233)
                printf("It is big end\n");
        else
                printf("I am confused\n");
        return 0;
}

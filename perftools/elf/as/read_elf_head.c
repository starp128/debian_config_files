#include <stdio.h>

#include <elf.h>

int main()
{
        unsigned int main_addr = 0;
        main_addr = (unsigned int)main;
        printf("addr of main is 0x%08x\n", main_addr);
        unsigned int page = main_addr/4096 * 4096;
	unsigned int offset = main_addr%4096;
	printf("page boundry main is 0x%08x\n", (unsigned int)page);
	printf("page offset of main is 0x%08x\n", (unsigned int)offset);

	Elf32_Ehdr* elf_head;
	elf_head = (Elf32_Ehdr*)page;
	unsigned char* addr_char = (unsigned char* )page;
	int i;
	for (i = 0; i < 54 ; i++) {
		printf("%02x ", *addr_char++);
	}
	printf("\n");
	
	printf("e_entry %08x", elf_head->e_entry);
}

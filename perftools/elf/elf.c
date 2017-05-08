#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <libelf.h>
#include <elf.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "comm.h"

char* version = "v0.2";

int dump_raw = 1;
int dump_content = 0;

int dump_header = 1;
int dump_shstrtab = 1;
int dump_section_header = 1;

int dump_strtab = 1;
int dump_symtab = 1;

int dump_comment = 0;
int dump_rodata = 1;
int dump_data = 1;
int dump_bss = 1;
int dump_text = 1;

int dump_pheader = 1;
int dump_dyn = 1;

/* Should not used in external. Only export function interface */

int in_fd;
static Elf32_Ehdr* elf_header; 	/* Top level in elf file */
static Elf32_Shdr* elf_shstrtab_header; /* Sub level */
static char* elf_shstrtab;

static int section_offset;
static int section_size;
static int section_num;
static Elf32_Shdr* elf_section_head[100]; /* Point to section in memory */

int strtab_index;
int symtab_index;
unsigned char* elf_ph_buf;
unsigned char* elf_strtab_buf;
unsigned char* elf_sym_buf;


Elf32_Dyn* elf_dyn;

void elf_header_dump()
{
	bin_buffer_reset();
	switch (elf_header->e_type)
	{
	case ET_NONE:
		/* num is the printed number of string *NOT* include the added '\0'.
		   so next time will overlap the trailing '\0'. That's we want.
		   so we do not sprintf the whole string by one sprintf call. Just
		   split them with serival calls.
		 */
		bin_printf(0, "unknown type\n");
		break;
	case ET_REL:
		bin_printf(0, "Relocable file\n");
		break;
	case ET_EXEC:
		bin_printf(0, "Executable file\n");
		break;
	case ET_DYN:
		bin_printf(0, "DYN file\n");
		break;
	default:
		bin_printf(0, "unknow\n");
	}

	if (elf_header->e_machine == EM_386) {
		bin_printf(0, "i386\n");
	} else if (elf_header->e_machine == EM_X86_64) {
		bin_printf(0, "x84_64\n");
	} else if (elf_header->e_machine == EM_S390) {
		bin_printf(0, "s390\n");
	} else {
		bin_printf(0, "other\n");
	}


	bin_printf(0, "entry %08lX\n", elf_header->e_entry);

	bin_printf(0, "PROGRAM HEADER\n");
	bin_printf(0, "OFFSET     : %X\n", (unsigned long long)elf_header->e_phoff);
	bin_printf(0, "ENTRY SIZE : %X\n", elf_header->e_phentsize);
	bin_printf(0, "ENTRY NUM  : %X\n", elf_header->e_phnum);


	bin_printf(0, "SECTION HEADER\n");
	bin_printf(0, "OFFSET     : %X\n", elf_header->e_shoff);
	bin_printf(0, "ENTRY SIZE : %X\n", elf_header->e_shentsize);
	bin_printf(0, "ENTRY NUM  : %X\n", elf_header->e_shnum);

	bin_printf(0, "shstrndx   : %X\n", elf_header->e_shstrndx);

	binary_dump_with_string(0, sizeof(Elf32_Ehdr), (unsigned char*)elf_header, bin_buffer());

}

void elf_section_header_dump(Elf32_Shdr* elf_section_header, char* section_name,
			     int offset_in_file)
{
	bin_printf(1, "NAME %s\n", section_name+elf_section_header->sh_name);
	bin_printf(0, "OFF %8x\n", elf_section_header->sh_offset);
	bin_printf(0, "SIZ %8x END %8x\n", elf_section_header->sh_size,
		elf_section_header->sh_offset + elf_section_header->sh_size);
	bin_printf(0, "add %08x\n", elf_section_header->sh_addr);

	binary_dump_with_string(offset_in_file,
				 sizeof(Elf32_Shdr),
				(unsigned char*)elf_section_header,
				bin_buffer()
		);
}

void _elf_shstrtab_dump(char* strtab, int size)
{
	int i = 0;
	char* p = strtab;
	int len;

	fprintf(stderr, "section head string table\n");
	while (i < size - 1)
	{
		while(*p == 0) {
			p++;
			i++;
		}
		fprintf(stderr, "index %4x %s\n", i, p);
		len = strlen(p);
		p += len;
		i += len;
	}
	fprintf(stderr, "\n");
}


char* elf_section_name(char* shstrtab, int index)
{
	return shstrtab + index;
}

int elf_find_shstrtab(Elf32_Shdr* elf_section_head[], int section_num,
		    char* elf_shstrtab, char* name)
{
	int i;
	char* p;
	for (i = 0; i < section_num; i++)
	{
		p = elf_shstrtab + elf_section_head[i]->sh_name;
		if (!strcmp(p, name)) {
			return i;
		}
	}
	return -1;
}

void elf_strtab_dump(char* strtab, int size)
{
	int i = 0;
	char* p = strtab;
	int len;

	fprintf(stderr, "string table\n");
	while (i < size - 1)
	{
		while(*p == 0) {
			p++;
			i++;
		}
		fprintf(stderr, "index %4x %s\n", i, p);
		len = strlen(p);
		p += len;
		i += len;
	}
	fprintf(stderr, "\n");
}

/* find symbol name */
char* elf_sym_name(char* strtab, int index)
{
	return strtab + index;
}

/* typedef struct { */
/* 	uint32_t      st_name; */
/* 	Elf32_Addr    st_value; */
/* 	uint32_t      st_size; */
/* 	unsigned char st_info; */
/* 	unsigned char st_other; */
/* 	uint16_t      st_shndx; */
/* } Elf32_Sym; */
void elf_dump_symtab(Elf32_Shdr* header, unsigned char* elf_sym_buf, unsigned char* strtab)
{
	int offset_in_file = header->sh_offset;
	int section_size = header->sh_size;
	int sym_size = header->sh_entsize;
	int sym_num = section_size / sym_size;

	int i;
	Elf32_Sym* sym;
	unsigned char* p;

	if (sym_size != sizeof(Elf32_Sym)) {
		exit(-1);
	}
	if (section_size % sym_size) {
		exit(-1);
	}

	fprintf(stderr, "Symbol size : %d\n", sym_size);
	fprintf(stderr, "Symbol num  : %d\n", sym_num);

	for (i = 0; i < sym_num; i++ ) {
		p = elf_sym_buf + i * sym_size;
		offset_in_file = offset_in_file + i * sym_size;
		sym = (Elf32_Sym* )p;
		fprintf(stderr, "name  %x (%s)\n", sym->st_name,
			elf_sym_name(strtab, sym->st_name));
		fprintf(stderr, "value %x size %x shndx %x\n",
			sym->st_value,
			sym->st_size,
			sym->st_shndx);
		fprintf(stderr, "offset(value) into section(shndx) with length(size)\n");
		fprintf(stderr, "info  %d (Type and Bind)\n", sym->st_info);
		fprintf(stderr, "other %d (Visiable)\n", sym->st_other);
		fprintf(stderr, "The Raw Data:\n\n");
		binary_dump(offset_in_file, sym_size, p);
		fprintf(stderr, "\n");

	}

	/* The most import symbol is OBJECT and FUNC.
	 */
}

void elf_rodata_dump(int section_index, Elf32_Shdr* head, unsigned char* buf, Elf32_Shdr* sym_head, unsigned char* sym_buf)
{
	int offset_in_file = head->sh_offset;
	int size = head->sh_size;

	binary_dump(offset_in_file, size, buf);
	/* Extra: map to symbol? seems no symbol related to this
	   Why is here? AFAK .rel.text use this.
	 */
}
char* _sym_bind(unsigned char n)
{
	char* ret = NULL;
	switch (n) {
	case STB_LOCAL:   /* Local symbol */
		ret = "LOCAL";
		break;
	case STB_GLOBAL:  /* Global symbol */
		ret = "GLOBAL";
		break;
	case STB_WEAK:    /* Weak symbol */
		ret = "WEAK";
		break;
	case STB_NUM:     /* Number of defined types.  */
		ret = "NUM";
		break;
	case STB_LOOS:    /* Start of OS-specific */
		ret = "LOOS";
		break;
/*	case STB_GNU_UNIQUE:
		ret = "GNU_UN";
		break;*/
	case STB_HIOS:    /* End of OS-specific */
		ret = "HIOS";
		break;
	case STB_LOPROC:  /* Start of processor-specific */
		ret = "LOPROC";
		break;
	case STB_HIPROC:  /* End of processor-specific */
		ret = "HIPROC";
		break;
	default:
		ret = "UNKNOW";
	}
	return ret;
}
char* _sym_type(unsigned char n)
{
	char* ret = NULL;
	switch (n) {
	case STT_NOTYPE:
		ret = "NOTYPE";
		break;
	case STT_OBJECT:
		ret = "OBJECT";
		break;
	case STT_FUNC:
		ret = "FUNC";
		break;
	case STT_SECTION:
		ret = "SECTION";
		break;
	case STT_FILE:
		ret = "FILE";
		break;
	case STT_COMMON:
		ret = "COMMON";
		break;
	case STT_TLS:
		ret = "TLS";
		break;
	case STT_NUM:
		ret = "NUM";
		break;
	case STT_LOOS:
		ret = "LOOS";
		break;
/*	case STT_GNU_IFUNC:
		ret = "GNU_IFU";
		break;*/
	case STT_HIOS:
		ret = "HIOS";
		break;
	case STT_LOPROC:
		ret = "LOPROC";
		break;
	case STT_HIPROC:
		ret = "HIPROC";
		break;
	default:
		ret = "UNKNOW";
	}
	return ret;
}

char* _sym_idx(unsigned short n)
{
	char* ret = NULL;
	char num[5];
	num[4] = 10;
	if (0 < n && n < 0xff00) {
		/* normal section number */
		sprintf(num, "%4d", n); /* sprintf will add '\0' */
		return strdup(num);
	}
	switch (n) {
	case SHN_UNDEF:
		ret = "UND";
		break;
	case SHN_ABS:
		ret = "ABS";
		break;
	case SHN_COMMON:
		ret = "COM";
		break;
	default:
		ret = " UN";
	}
	return ret;
}
void elf_data_dump(int data_index, Elf32_Shdr* head, unsigned char* buf, Elf32_Shdr* sym_head, unsigned char* sym_buf, unsigned char* str_buf)
{
	int offset_in_file = head->sh_offset;
	int size = head->sh_size;

	int i, j = 0;
	int sym_section_size = sym_head->sh_size;
	int sym_size = sym_head->sh_entsize;
	int sym_num;
	Elf32_Sym* sym;
	char* idx;

	if (sym_size != sizeof(Elf32_Sym)) {
		exit(-1);
	}
	sym_num = sym_section_size / sym_size;
	/* Extra: map to symbol? Yes */
	fprintf(stderr, "Symbol contained in .data section\n");
	fprintf(stderr, "   Num:   offset     Size     Tpye     Bind Name\n");
	for (sym = (Elf32_Sym*)sym_buf, i = 0; i < sym_num; i++, sym++) {
		/* Find symbol relate to this section */
		if (sym->st_shndx == data_index) {
			fprintf(stderr, "%6d: %08x %8x %8s %8s %s\n",
				j++,
				sym->st_value, sym->st_size,
				_sym_type(SYM_TYPE(sym->st_info)),
				_sym_bind(SYM_BIND(sym->st_info)),
				elf_sym_name(str_buf, sym->st_name));
		}
	}
	fprintf(stderr, "Raw data :\n");
	binary_dump(offset_in_file, size, buf);
}

/* thought nothing in file, but dump the symbol in the section */
void elf_bss_dump(int bss_index, Elf32_Shdr* sym_head, unsigned char* sym_buf,
		  unsigned char* str_buf)
{
	int i, j = 0;
	int sym_section_size = sym_head->sh_size;
	int sym_size = sym_head->sh_entsize;
	int sym_num;
	Elf32_Sym* sym;

	if (sym_size != sizeof(Elf32_Sym)) {
		exit(-1);
	}
	sym_num = sym_section_size / sym_size;
	fprintf(stderr, "Symbol contained in .bss section\n");
	fprintf(stderr, "   Num:   offset     Size     Tpye     Bind Name\n");
	for (sym = (Elf32_Sym*)sym_buf, i = 0; i < sym_num; i++, sym++) {
		/* Find symbol relate to this section */
		if (sym->st_shndx == bss_index) {
			fprintf(stderr, "%6d: %08x %8x %8s %8s %s\n",
				j++,
				sym->st_value, sym->st_size,
				_sym_type(SYM_TYPE(sym->st_info)),
				_sym_bind(SYM_BIND(sym->st_info)),
				elf_sym_name(str_buf, sym->st_name));
		}
	}
	fprintf(stderr, ".bss has no data in file\n\n");
}

void elf_text_dump(int text_index, Elf32_Shdr* head, unsigned char* buf,
		   Elf32_Shdr* sym_head, unsigned char* sym_buf,
		   unsigned char* str_buf)
{
	int offset_in_file = head->sh_offset;
	int size = head->sh_size;

	int i, j = 0;
	int sym_section_size = sym_head->sh_size;
	int sym_size = sym_head->sh_entsize;
	int sym_num;
	Elf32_Sym* sym;

	if (sym_size != sizeof(Elf32_Sym)) {
		exit(-1);
	}
	sym_num = sym_section_size / sym_size;
	/* Extra: map to symbol? Yes */
	fprintf(stderr, "Symbol contained in .text section\n");
	fprintf(stderr, "   Num:   offset     Size     Tpye     Bind Name\n");
	for (sym = (Elf32_Sym*)sym_buf, i = 0; i < sym_num; i++, sym++) {
		/* Find symbol relate to this section */
		if (sym->st_shndx == text_index) {
			fprintf(stderr, "%6d: %08x %8x %8s %8s %s\n",
				j++,
				sym->st_value, sym->st_size,
				_sym_type(SYM_TYPE(sym->st_info)),
				_sym_bind(SYM_BIND(sym->st_info)),
				elf_sym_name(str_buf, sym->st_name));
		}
	}
	fprintf(stderr, "Raw data :\n");
	binary_dump(offset_in_file, size, buf);
}

char* _ph_type(unsigned int type)
{
	char* ret = NULL;
	switch (type) {
	case PT_NULL:
		ret = "NULL";
		break;
	case PT_LOAD:
		ret = "LOAD";
		break;
	case PT_DYNAMIC:
		ret = "DYN";
		break;
	case PT_INTERP:
		ret = "INTERP";
		break;
	case PT_NOTE:
		ret = "NOTE";
		break;
	case PT_SHLIB:
		ret = "SHLIB";
		break;
	case PT_PHDR:
		ret = "PHDR";
		break;
	case PT_TLS:
		ret = "TLS";
		break;
	case PT_NUM:
		ret = "NUM";
		break;
	default:
		ret = "UNK";
	}
	return ret;
}
void elf_ph_dump(Elf32_Ehdr* elf_header, unsigned char* buf)
{
	int p_offset = elf_header->e_phoff;
	int p_size = elf_header->e_phentsize;
	int p_num = elf_header->e_phnum;

	Elf32_Phdr* p = (Elf32_Phdr*)buf;
	int i;
	char* type;
	fprintf(stderr, "\nProgram Headers: offset:0x%08x size 0x%x (%d)\n", p_offset,
		p_size * p_num, p_size * p_num);
	fprintf(stderr, "TYPE\t\t Offset   BirtAddr   PhyAddr    Flg Align\n");
	for (i = 0; i < p_num; i++, p++) {
		type = _ph_type(p->p_type);
		fprintf(stderr, "%s\t\t 0x%06x 0x%08x 0x%08x 0x%05x 0x%05x\n", type,
			p->p_offset, p->p_vaddr, p->p_paddr, p->p_filesz, p->p_memsz);
	}
	binary_dump(p_offset, p_size * p_num, buf);
}
void elf_ph_dump2(Elf32_Ehdr* elf_header, unsigned char* buf)
{
	int p_offset = elf_header->e_phoff;
	int p_size = elf_header->e_phentsize;
	int p_num = elf_header->e_phnum;
	int offset_in_file;

	Elf32_Phdr* p = (Elf32_Phdr*)buf;
	int i;
	char* type;

	for (i = 0; i < p_num; i++, p++) {
		bin_buffer_reset();
		type = _ph_type(p->p_type);
		bin_printf(0, "%s\n", type);
		bin_printf(0, "0x%06x\n", p->p_vaddr);
		bin_printf(0, "0x%06x\n", p->p_paddr);
		offset_in_file = p_offset + p_size * i;
		binary_dump_with_string(offset_in_file, p_size,
					(unsigned char*)p, bin_buffer());
	}
}
void elf_summary_symtab(Elf32_Shdr* header, unsigned char* sym_buf, unsigned char* str_buf)
{
	int offset_in_file = header->sh_offset;
	int section_size = header->sh_size;
	int sym_size = header->sh_entsize;
	int sym_num = section_size / sym_size;

	int i, j = 0;
	Elf32_Sym* sym;

	if (sym_size != sizeof(Elf32_Sym)) {
		exit(-1);
	}
	if (section_size % sym_size) {
		exit(-1);
	}


	fprintf(stderr, "   Num:   offset     Size     Tpye     Bind  Ndx Name\n");
	for (sym = (Elf32_Sym*)sym_buf, i = 0; i < sym_num; i++, sym++) {
		fprintf(stderr, "%6d: %08x %8x %8s %8s %4s %s\n",
			j++,
			sym->st_value, sym->st_size,
			_sym_type(SYM_TYPE(sym->st_info)),
			_sym_bind(SYM_BIND(sym->st_info)),
			_sym_idx(sym->st_shndx),
			elf_sym_name(str_buf, sym->st_name));
	}
}

char* _dyn_type(int n)
{
	char* ret = NULL;
	switch (n) {
	case DT_NULL:
		ret = "NULL";
		break;
	case DT_NEEDED:
		ret = "NEEDED";
		break;
	case DT_PLTRELSZ:
		ret = "PLTRELSE";
		break;
	case DT_PLTGOT:
		ret = "PLTGOT";
		break;
	case DT_HASH:
		ret = "HASH";
		break;
	case DT_STRTAB:
		ret = "STRTAB";
		break;
	case DT_SYMTAB:
		ret = "SYMTAB";
		break;
	case DT_RELA:
		ret = "RELA";
		break;
	case DT_RELASZ:
		ret = "RELASZ";
		break;
	case DT_RELAENT:
		ret = "RELAENT";
		break;
	case DT_STRSZ:
		ret = "STRSZ";
		break;
	case DT_SYMENT:
		ret = "SYMENT";
		break;
	case DT_INIT:
		ret = "INIT";
		break;
	case DT_FINI:
		ret = "FINI";
		break;
	case DT_SONAME:
		ret = "SONAME";
		break;
	case DT_RPATH:
		ret = "RPATH";
		break;
	case DT_SYMBOLIC:
		ret = "SYMBOLIC";
		break;
	case DT_REL:
		ret = "REL";
		break;
	case DT_RELSZ:
		ret = "RELSZ";
		break;
	case DT_RELENT:
		ret = "RELENT";
		break;
	case DT_PLTREL:
		ret = "PLTREL";
		break;
	case DT_DEBUG:
		ret = "DEBUG";
		break;
	case DT_TEXTREL:
		ret = "TEXTREL";
		break;
	case DT_JMPREL:
		ret = "JMPREL";
		break;
	case DT_BIND_NOW:
		ret = "BIND_NOW";
		break;
	case DT_INIT_ARRAY:
		ret = "INIT_ARRAY";
		break;
	case DT_FINI_ARRAY:
		ret = "FINI_ARRAY";
		break;
	case DT_INIT_ARRAYSZ:
		ret = "INIT_ARRAYSZ";
		break;
	case DT_FINI_ARRAYSZ:
		ret = "FINI_ARRAYSZ";
		break;
	case DT_RUNPATH:
		ret = "RUNPATH";
		break;
	case DT_FLAGS:
		ret = "FLAGS";
		break;
	case DT_ENCODING:
		ret = "ENCODING";
		break;
	/* case DT_PREINIT_ARRAY: */
	/* 	ret = "PREINIT_ARRAY"; */
	/* 	break; */
	case DT_PREINIT_ARRAYSZ:
		ret = "PREINIT_ARRAYSZ";
		break;
	case DT_NUM:
		ret = "NUM";
		break;
	case DT_LOOS:
		ret = "LOOS";
		break;
	case DT_HIOS:
		ret = "HIOS";
		break;
	case DT_LOPROC:
		ret = "LOPROC";
		break;
	case DT_HIPROC:
		ret = "HIPROC";
		break;
	default:
		ret = "UNK";
		break;
	}
	return ret;
}

void elf_dynamic_dump(Elf32_Shdr* header, unsigned char* buf)
{
	int offset_in_file = header->sh_offset;
	int size = header->sh_size;
	int num = size / sizeof(Elf32_Dyn);
	Elf32_Dyn* dyn = (Elf32_Dyn*)buf;
	int i;
	for (i = 0; i < num; i++, dyn++) {
		bin_printf(1, "Dyn %d\n", i);
		bin_printf(0, "%s\n", _dyn_type(dyn->d_tag));
		bin_printf(0, "%08x", dyn->d_un.d_val);
		offset_in_file = header->sh_offset + i * sizeof(Elf32_Dyn);
		binary_dump_with_string(offset_in_file, sizeof(Elf32_Dyn),
					(unsigned char*)dyn, bin_buffer());
	}

}

void* mem_get(unsigned int size)
{
	return malloc(size);
}

/* Should we manager memory allocation oulself
 * or depend the caller.

 * We do not need the file descripter, as now it ok with the 'global' fd

 * We do not allow error. read or die. That means avoid such read error by caller.
       Maybe, return error is better, let the caller die or struggle
 */
unsigned char* elf_file_read(int offset, int size)
{
	unsigned char* ret = NULL;
	int tmp;
	ret = mem_get(size);
	if (!ret) {
		perr("no memory");
		return NULL;
	}
	tmp = lseek(in_fd, offset, SEEK_SET);
	if (lseek(in_fd, offset, SEEK_SET) != offset) {
		perr("lseek");
		return NULL;
	}
	if (read(in_fd, ret, size) != size) {
		perr("read");
		return NULL;
	}
	return ret;

}

void _elf_section_dump_by_name(char* name)
{
	int index;
	int offset;
	int size;
	unsigned char* data;
	index = elf_find_shstrtab(elf_section_head, section_num,
					 elf_shstrtab, name);
	offset = elf_section_head[index]->sh_offset;
	size = elf_section_head[index]->sh_size;
	data = elf_file_read(offset, size);
	binary_dump(offset, size, data);

}
/* Response for all the io with the disk */
void elf_init_read()
{
	int i;

	/* Dump elf header */
	elf_header = (Elf32_Ehdr*)elf_file_read(0, sizeof(*elf_header));

	section_offset = elf_header->e_shoff;
	section_size = elf_header->e_shentsize;
	section_num = elf_header->e_shnum;

	/* Read section name */
	elf_shstrtab_header = (Elf32_Shdr*)elf_file_read(
			section_offset + elf_header->e_shstrndx * section_size,
			section_size);
	if (elf_shstrtab_header->sh_type != SHT_STRTAB) {
		perr("err");
	}
	elf_shstrtab = elf_file_read(elf_shstrtab_header->sh_offset,
				     elf_shstrtab_header->sh_size);

	/* Read Section Header  */
	for (i = 0; i < section_num; i++) {
		elf_section_head[i] = (Elf32_Shdr*)elf_file_read(section_offset +
								 section_size * i,
								 section_size);
	}
}

void _elf_file_dump(int in_fd)
{

	int i;
	int ret;
	int offset_in_file = 0;

	if (dump_header) {
		elf_header_dump(elf_header);
	}

	if (dump_section_header) {
		fprintf(stderr, "===================Section===============\n");
		fprintf(stderr, "Size Section Header %d\n", sizeof(Elf32_Shdr));

		for (i = 0; i < section_num; i++)
		{
			offset_in_file = section_offset + i * section_size;
			elf_section_header_dump(elf_section_head[i],
						elf_shstrtab,
						offset_in_file);
		}
	}

	/* Dump Strtab */

	strtab_index = elf_find_shstrtab(elf_section_head, section_num,
					 elf_shstrtab, ".strtab");
	elf_strtab_buf = elf_file_read(elf_section_head[strtab_index]->sh_offset,
				       elf_section_head[strtab_index]->sh_size);
	if (dump_strtab) {
		//elf_strtab_dump(elf_strtab_buf, elf_section_head[strtab_index]->sh_size);
		binary_dump_with_char(elf_section_head[strtab_index]->sh_offset,
				      elf_section_head[strtab_index]->sh_size,
				      elf_strtab_buf);
	}


	/* Dump Symbol Table. Always read in symbol no matter whether dump or not
	   We may use is later.
	 */
	symtab_index = elf_find_shstrtab(elf_section_head, section_num,
					 elf_shstrtab, ".symtab");
	elf_sym_buf = elf_file_read(elf_section_head[symtab_index]->sh_offset,
				    elf_section_head[symtab_index]->sh_size);

	if (dump_symtab) {
		elf_dump_symtab(elf_section_head[symtab_index],	elf_sym_buf, elf_strtab_buf);
	}

	/* +++++++++++++++++++++++++++FINISH SYNBOL+++++++++++++++++++++++++++++++++++++ */
	GAP("THE CONTENT OF TEXT DATA BSS RODATA");


	/* Todo check if this section exist */
	if (dump_rodata) {
		_elf_section_dump_by_name(".rodata");
	}
	if (dump_data) {
		_elf_section_dump_by_name(".data");
	}
	if (dump_bss) {
		_elf_section_dump_by_name(".bss");
	}
	if (dump_text) {
		_elf_section_dump_by_name(".text");
	}
	if (dump_dyn) {
		int dyn_index;
		unsigned char* dyn_buf;
		dyn_index = elf_find_shstrtab(elf_section_head,
					      section_num,
					      elf_shstrtab,
					      ".dynamic");
		dyn_buf = elf_file_read(elf_section_head[dyn_index]->sh_offset,
					elf_section_head[dyn_index]->sh_size);
		elf_dynamic_dump(elf_section_head[dyn_index], dyn_buf);
	}
}

int elf_dump_summary()
{
	/* Summary */
	int section_dumped, i;
	fprintf(stderr, "\n\nSummary\n\n");
	fprintf(stderr, "Not dumped section :\n       index name \n");
	for (i = 0; i < section_num; i++) {
		section_dumped = 0;
		if (i == elf_header->e_shstrndx) {
			section_dumped = 1;
		}
		if ( dump_strtab && i == strtab_index) {
			section_dumped = 1;
		}
		if ( dump_symtab && i == symtab_index) {
			section_dumped = 1;
		}

		char* name = elf_section_name(elf_shstrtab, elf_section_head[i]->sh_name);
		/* No need to check two section with the same name */
		if (dump_comment) {
			if (!strcmp(".comment", name)) {
				section_dumped = 1;
			}
		}
		if (dump_rodata) {
			if (!strcmp(".rodata", name))
				section_dumped = 1;
		}
		if (dump_data) {
			if (!strcmp(".data", name))
				section_dumped = 1;
		}
		if (dump_bss) {
			if (!strcmp(".bss", name))
				section_dumped = 1;
		}
		if (dump_bss) {
			if (!strcmp(".text", name))
				section_dumped = 1;
		}

		if (section_dumped) {
			fprintf(stderr, "%s %8d %s\n", "YES", i, name);
		} else {
			fprintf(stderr, "%s %8d %s\n", "   ", i, name);
		}

	}

	elf_summary_symtab(elf_section_head[symtab_index], elf_sym_buf, elf_strtab_buf);
}

void elf_file_dump(char* elf_file_name)
{
	in_fd = open(elf_file_name, O_RDONLY);
	if (in_fd < 0) {
		exit(-1);
	}
	elf_init_read();
	_elf_file_dump(in_fd);
	free(elf_shstrtab);
	free(elf_strtab_buf);
	free(elf_sym_buf);
}

void elf_liner_dump(char* elf_file_name)
{
	int i;
	int off, sz;
	unsigned char* buf;

	in_fd = open(elf_file_name, O_RDONLY);
	if (in_fd < 0) {
		exit(-1);
	}

	elf_init_read();
	if (dump_header) {
		elf_header_dump(elf_header);
	}

	int p_offset = elf_header->e_phoff;
	int p_size = elf_header->e_phentsize;
	int p_num = elf_header->e_phnum;

	elf_ph_buf = elf_file_read(p_offset, p_size * p_num);
	if (dump_pheader) {
		elf_ph_dump2(elf_header, elf_ph_buf);
	}

	for (i = 0; i < section_num; i++) {
		if (i == 0 ) {
			continue;
		}
		off = elf_section_head[i]->sh_offset;
		sz = elf_section_head[i]->sh_size;
		buf = elf_file_read(off, sz);
		bin_buffer_reset();
		bin_printf(0, "%s\n", elf_section_name(elf_shstrtab,
				   elf_section_head[i]->sh_name));
		binary_dump_with_string(off, sz, buf, bin_buffer());
		free(buf);
	}
}

void usage()
{
	fprintf(stderr, "cmd [-r|-s] inputfile\n");
	fprintf(stderr, "    -r dump out all raw data\n");
	fprintf(stderr, "    -s dump out all readable context\n\n");
}

void show_version()
{
	fprintf(stderr, "%s\n", version);
}
int main(int argc, char** argv)
{
	char option;
	char* inputfile;

	if (argc!=3){
		usage();
	}
        while((option = getopt(argc,argv,"hvz:rs"))!=-1)
        {
                switch(option)
                {
                case 'h':
                        usage();
                        return 0;
                case 'v':
                        show_version();
                        return 0;
                case 'z':
                        //printf("%s\n",optarg);
                        //printf("%lx\n",optarg);
                        //printf("%x\n",&optarg);
                        //arg_skip = atoi(optarg);
                        break;
		case 'r':
			dump_raw = 1;
			break;
		case 's':
			dump_raw = 0;
			dump_content = 1;
			break;
                default:
                        printf("unknow option %c",option);
			usage();
                        return 1;
                }
        }

	inputfile = argv[optind];

	if (dump_raw) {
		dump_header = 1;
		dump_pheader = 1;
		elf_liner_dump(argv[optind]);
		return 0;
	}

	if (dump_content) {
		elf_file_dump(argv[optind]);
	}

	return 0;
}



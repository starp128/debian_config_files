/* 
   This is a library for dig into elf file.

   Dependence : libc
   API: the list can be found in the header file
        The style is:
	In      give the elf object. every elf file is represented by in iternal object.
		the object in fact is big fat global variable. But not intend for the
		direct use by the user. We suggest using the function provided.
	Out	return the needed object
	error	set the global error variable. and return -1. like many libc function.
		user can use other functon to print the value reason.(not available now)
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elf.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "comm.h"

#define elf_malloc malloc
#define elf_free free

/* All original data is here. But it is not suggested to use them directly.
   It's better to use the standard function.

   We assume there is only one section with a specific section name.
   Otherwise something need to done, like wrap the duplicated section, ie,
   .shstrtab to a struct in order to link the other section with same name
       struct strtab {
       Elf32_shstrtab* strtab;
       struct strtab * next;
       }
*/

char _EMPTY = 0; 			/* a safe place for pointer*/
char* EMPTY = &_EMPTY;

struct elf_obj {
	int fd;
	Elf32_Ehdr* head;

	Elf32_Shdr* section; 	/* Special section. Section head sub struct */
	int section_num;

	Elf32_Sym* sym;  	/* Special section. Symbol sub struct */
	int sym_num;

	Elf32_Sym* dynsym;	/* Special */
	int dynsym_num;

	Elf32_Dyn* dyn;
	int dyn_num;
	
	char* shstrtab;		/* below is not expected directly use */
	int shstrtab_len;
	char* strtab;
	int strtab_len;
	char* dynstr;
	int dynstr_len;
};

static unsigned char* elf_file_read(struct elf_obj* obj, int offset, int size)
{
	unsigned char* ret = NULL;
	int tmp;
	int in_fd = obj->fd;
	ret = elf_malloc(size);
	if (!ret) {
		perr("no memory");
		return NULL;
	}

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

static int elf_section_head_read(struct elf_obj* obj)
{
	Elf32_Ehdr* elf_header;
	int section_offset;
	int section_size;
	int section_num;
	char* elf_shstrtab;
	Elf32_Shdr* elf_section_head;

	elf_header = (Elf32_Ehdr*)elf_file_read(obj, 0, sizeof(*elf_header));
	section_offset = elf_header->e_shoff;
	section_size = elf_header->e_shentsize;
	section_num = elf_header->e_shnum;
	elf_section_head = (Elf32_Shdr*)elf_file_read(obj,
						   section_offset,
						   section_size * section_num);
	obj->head = elf_header;
	obj->section = elf_section_head;
	obj->section_num = section_num;

	elf_section_head += elf_header->e_shstrndx; /* move to shstrtab */
	elf_shstrtab = elf_file_read(obj,
				     elf_section_head->sh_offset,
				     elf_section_head->sh_size);
	obj->shstrtab = elf_shstrtab;
	obj->shstrtab_len = elf_section_head->sh_size;
	return 0;
}


/***********************************************/
/* Section */
char* elf_section_name(struct elf_obj* obj, int index)
{
	int max_length;

	max_length = obj->shstrtab_len;
	char* shstrtab = obj->shstrtab;
	if (index > max_length) {
		return shstrtab; /* That is a safe place */
	}
	return shstrtab + index;
}

Elf32_Shdr* elf_section_head(struct elf_obj* obj, char* name)
{
	int i;
	char* p;
	int section_num = obj->section_num;
	Elf32_Shdr* elf_section_head = obj->section;
	char* elf_shstrtab = obj->shstrtab;

	for (i = 0; i < section_num; i++, elf_section_head++)
	{
		p = elf_shstrtab + elf_section_head->sh_name;
		if (!strcmp(p, name)) {
			return elf_section_head;
		}
	}
	return NULL;
}

unsigned char* _elf_section(struct elf_obj* obj, Elf32_Shdr* section_head)
{
	int offset = section_head->sh_offset;
	int len = section_head->sh_size;
	return elf_file_read(obj, offset, len);
}

unsigned char* elf_section(struct elf_obj* obj, char* name)
{
	Elf32_Shdr* section;
	section = elf_section_head(obj, name);
	return _elf_section(obj, section);
}

/* Should we provide this? Seems no need */
/* Oh get a reason. section is referenced by index in symbol */
char* elf_section_index()
{
}

Elf32_Shdr* elf_section_head_by_index(struct elf_obj* obj, int index)
{
	Elf32_Shdr* section;
	if (!obj)
		return NULL;
	if (index >= obj->section_num) {
		return NULL;
	}
	section = obj->section;
	section += index;
	return section;
}
unsigned char* elf_section_by_index()
{
}

void elf_for_each_section(struct elf_obj* obj,
				    void (*callback)(Elf32_Shdr*, unsigned char*)
	)
{
	int i;
	Elf32_Shdr* section_head = obj->section;
	int section_num = obj->section_num;
	unsigned char* section_data;
	for (i = 0; i < section_num; i++, section_head++) {
		section_data = _elf_section(obj, section_head);
		(*callback)(section_head, section_data);
		elf_free(section_data);
	}
}
/********************************************************/
/* Symbol */
int elf_sym_read(struct elf_obj* obj)
{
	char* strtab = NULL;
	char* sym;
	Elf32_Shdr* sym_head;

	if (!obj)
		return -1;
	strtab = elf_section(obj, ".strtab");
	if (!strtab) {
		perr("can not get .strtab section");
		return -1;
	}
	obj->strtab = strtab;
	
	sym_head = elf_section_head(obj, ".symtab");
	if (!sym_head) {
		perr("sym tab");
		return -1;
	}
	obj->sym_num = sym_head->sh_size / sizeof(Elf32_Sym);
	sym = _elf_section(obj, sym_head);
	obj->sym = (Elf32_Sym*)sym;
	return 0;
}

char* elf_sym_name(struct elf_obj* obj, int index)
{
	return obj->strtab + index;
}

void elf_for_each_sym(struct elf_obj* obj,
		      void (*callback)(struct elf_obj*, Elf32_Sym*))
{
	int i;
	int sym_num = obj->sym_num;
	Elf32_Sym* sym = obj->sym;

	for (i = 0; i < sym_num; i++, sym++) {
		(*callback)(obj, sym);
	}
}

void elf_sym_show_all(struct elf_obj* obj, Elf32_Sym* sym)
{
	char* name;
	char* section_name;
	Elf32_Shdr* sh;

	sh = elf_section_head_by_index(obj, sym->st_shndx);
	if (sh) {
		section_name = elf_section_name(obj, sh->sh_name);
	} else {
		section_name = EMPTY;
	}
	name = elf_sym_name(obj, sym->st_name);
	fprintf(stderr, "value %08x size %08x shndx %16s name %s\n",
		sym->st_value,
		sym->st_size,
		section_name,
		name);
}

void elf_sym_show_func(struct elf_obj* obj, Elf32_Sym* sym)
{
	char* name;
	char* section_name;
	Elf32_Shdr* sh;
	unsigned char type;
	char* flag;

	type = ELF32_ST_TYPE(sym->st_info);
	if (type != STT_FUNC) {
		return;
	}
	if (sym->st_size) {
		flag = "*";
	} else {
		flag = "\0";
	}
	
	sh = elf_section_head_by_index(obj, sym->st_shndx);
	if (sh) {
		section_name = elf_section_name(obj, sh->sh_name);
	} else {
		section_name = EMPTY;
	}
	name = elf_sym_name(obj, sym->st_name);
	fprintf(stderr, "%02s value %08x size %08x %16s name %s\n",
		flag,
		sym->st_value,
		sym->st_size,
		section_name,
		name);
}

int elf_dynsym_read(struct elf_obj* obj)
{
	char* dynstr = NULL;
	char* dynsym;
	Elf32_Shdr* dynsym_head;

	if (!obj)
		return -1;
	dynstr = elf_section(obj, ".dynstr");
	if (!dynstr) {
		perr("can not get .dynstr section");
		obj->dynstr = NULL;
		return 0;
	}
	obj->dynstr = dynstr;
	
	dynsym_head = elf_section_head(obj, ".dynsym");
	if (!dynsym_head) {
		perr("no sym tab");
		obj->dynsym = NULL;
		obj->dynsym_num = 0;
		return 0;
	}
	obj->dynsym_num = dynsym_head->sh_size / sizeof(Elf32_Sym);
	dynsym = _elf_section(obj, dynsym_head);
	obj->dynsym = (Elf32_Sym*)dynsym;
	return 0;
}

char* elf_dynsym_name(struct elf_obj* obj, int index)
{
	return obj->dynstr + index;
}

void elf_for_each_dynsym(struct elf_obj* obj,
			 void (*callback)(struct elf_obj*, Elf32_Sym*))
{
	int i;
	int sym_num = obj->dynsym_num;
	Elf32_Sym* sym = obj->dynsym;

	for (i = 0; i < sym_num; i++, sym++) {
		(*callback)(obj, sym);
	}
}

void elf_dynsym_show_all(struct elf_obj* obj, Elf32_Sym* sym)
{
	char* name;
	char* section_name;
	Elf32_Shdr* sh;

	sh = elf_section_head_by_index(obj, sym->st_shndx);
	if (sh) {
		section_name = elf_section_name(obj, sh->sh_name);
	} else {
		section_name = EMPTY;
	}
	name = elf_dynsym_name(obj, sym->st_name);
	fprintf(stderr, "value %08x size %08x shndx %16s name %s\n",
		sym->st_value,
		sym->st_size,
		section_name,
		name);
}

/*********************************************************/
/* Dynamic section */
int elf_dyn_read(struct elf_obj* obj)
{
	Elf32_Shdr* sh;
	int entsize;
	int offset;
	int size;
	int num;
	unsigned char* dyn;
	
	if (!obj)
		return -1;
	sh = elf_section_head(obj, ".dynamic");
	if (!sh) {
		perr("no .dynamic section");
		return -1;
	}
	entsize = sh->sh_entsize;
	if (entsize) {
		if (entsize != sizeof(Elf32_Dyn))
			return -1;
	}

	size = sh->sh_size;
	num = size / entsize;
	dyn = _elf_section(obj, sh);
	
	obj->dyn = (Elf32_Dyn*)dyn;
	obj->dyn_num = num;
	return 0;
}

extern char* _dyn_type(int n);
void elf_for_each_dyn(struct elf_obj* obj,
			 void (*callback)(struct elf_obj*, Elf32_Dyn*))
{
	int i;
	Elf32_Dyn* dyn = obj->dyn;
	for (i = 0; i < obj->dyn_num; i++, dyn++) {
		if (dyn->d_tag == 0) {
			break;	/* DT_NULL marks the end of the dyn */
		}
		(*callback)(obj, dyn);
	}
}

void elf_dyn_show_all(struct elf_obj* obj, Elf32_Dyn* dyn)
{
	fprintf(stderr, "%08x", dyn->d_tag);
	fprintf(stderr, "%16s", _dyn_type(dyn->d_tag));
	fprintf(stderr, "  %08x\n", dyn->d_un.d_val);
}

struct elf_obj* elf_init(char* elf_file_name)
{
	struct elf_obj* obj;
	int fd;
	struct stat in_stat;
	fd = open(elf_file_name, O_RDONLY);
	if (fd < 0) {
		return NULL;
	}
	if (fstat(fd, &in_stat) < 0) {
                return NULL;
        }
        if( !in_stat.st_size) {
                return NULL;
        }
	obj = elf_malloc(sizeof(struct elf_obj));
	if (!obj) {
		return NULL;
	}
	obj->fd = fd;
	elf_section_head_read(obj);

	elf_sym_read(obj); 	/* load sym  */
	elf_dynsym_read(obj);
	elf_dyn_read(obj);
	return obj;
}

void elf_fini(struct elf_obj* obj)
{
	if (obj->fd)
		close(obj->fd);
	if (obj->head)
		elf_free(obj->head);
	if (obj->section)
		elf_free(obj->section);
	if (obj->sym)
		elf_free(obj->sym);
	if (obj->shstrtab)
		elf_free(obj->shstrtab);
	if (obj->strtab)
		elf_free(obj->strtab);
	if (obj->dyn)
		elf_free(obj->dyn);
}
/* End of the library function */

int main(int argc, char* argv[])
{
	struct elf_obj* elf_obj;
	if (argc != 2 ) {
		perr("Usage: %s inputfile", argv[0]);
		return -1;
	}
	elf_obj = elf_init(argv[1]);

	elf_for_each_sym(elf_obj, elf_sym_show_all);
	elf_for_each_dynsym(elf_obj, elf_dynsym_show_all);
	elf_for_each_dyn(elf_obj, elf_dyn_show_all);

	/* seems 
	   .rel.dyn is for .got 
	   .rel.plt is for .got.plt
	 */
	elf_fini(elf_obj);
	return 0;
}

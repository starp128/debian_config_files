#include "comm.h"
#include <elf.h>

char* _dyn_type(int n)
{
	char* ret = 0;
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

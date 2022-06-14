#ifndef _COMM_H
#define _COMM_H

#define DEBUG	1
#ifdef  DEBUG
#define pdebug(fmt,args...) printf(fmt, ##args)
#else
#define pdebug(fmt,args...) /*do nothing */
#endif

#define ERR_DEBUG        1
#ifdef  ERR_DEBUG
#define perr(fmt,args...) printf("\nFile:<%s> Fun:[%s] Line:%d\n "fmt, __FILE__, __FUNCTION__, __LINE__, ##args)
#else
#define perr(fmt,args...) /*do nothing */
#endif


#include "elfile.h"
#include "binary_dump.h"


#endif 	/* _COMM_H */

#ifndef DEBUG_H
#define DEBUG_H

#define DEBUG_SWITCH 1
#ifdef DEBUG_SWITCH
#define pr_debug(fmt, args...) printf(fmt, ##args)
#else
#define pr_debug(fmt, args...) 	/* nothing */
#endif

#define pr_info(fmt, args...) printf(fmt, ##args)
#define pr_err(fmt, args...) fprintf(stderr, fmt, ##args)

#define pr_err_exit(fmt, args...)	\
        do {                            \
	pr_err(fmt, ##args);		\
	exit(-1);			\
        } while (0)

/* dynmic debug message */
extern int verbose;
#define pr_info_verb(fmt, args...)	\
        do {                            \
	if (verbose)                    \
		pr_info(fmt, ##args);    \
        } while (0)

#endif

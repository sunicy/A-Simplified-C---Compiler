#ifndef _STD_H_
#define _STD_H_

/* malloc it!*/
#define MALLOC(type) ((type*)(malloc(sizeof(type))))

//#define DEBUG
#ifdef DEBUG
	#define printd printf
#else
	#define printd print_null
#endif

#define TRUE 1
#define FALSE 0

/* Grammer Tree Root */
extern struct gnode* gtroot; /* defined in main.c */

/* print error message */
void print_err(int errtype, int lineno, char* msg);

/* error status setter! */
void set_error_status();

/* debug print! */
void print_null(char* s, ...);

#endif

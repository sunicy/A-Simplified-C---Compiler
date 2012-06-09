#include <stdio.h>
#include "stable.h"
#include "gtree.h"
#include "semantic.h"
#include "interm.h"

extern int yydebug;

char usage[] =	"Usage:\n"
				"\tcmmc <Source File Name> [[Dest File Name] [Whether to simplify]]\n"
				"\tOutput via stdout if destination file not specified.\n"
				"\t'no' if simplification not expected, 0-9 for levels.\n"
				"\t'whether to simplify' is available iff. Dest File Name is specified, and accessable.\n";

/* Grammer Tree Root */
struct gnode* gtroot;

static int err_status = 0;

inline void set_error_status()
{
	err_status = 1;
}

void print_err(int errtype, int lineno, char* msg)
{
	printf("Error type %d at line %d: %s\n", errtype, lineno, msg);
	set_error_status(); /* error has been found! */
}

int main(int argc, char** argv)
{
	int print_reduced;
	FILE *f;
	FILE *fout; /* stdout default */
	if (argc < 2)
	{
		printf("%s", usage);
		return -1;
	}

	if (argc < 4)
		print_reduced = 100;
	else if (strcmp(argv[3], "no") == 0)
		print_reduced = 0;
	else
		print_reduced = atoi(argv[3]);
	
	
	if (!(f = fopen(argv[1], "r")))
		return -1;

	if (argc < 3 || !(fout = fopen(argv[2], "w")))
		fout = NULL;

	/* initialization */
	init_stable();

	/* grammer */
	yyrestart(f);
	yyparse();

	if (gtroot == NULL)
	{
		printf("Syntax error, program stopped.\n");
		return -1;
	}

	/* semantic analysis */
	semantic_parse();

	if (err_status) /* Well, semantic error! */
		return -1;
	
	/* generate intermediate code! */
	im_init(fout);	
	interm_parse();

	if (!print_reduced)
		im_display();
	/* reduce the intermediate code! */
	im_reduce(print_reduced);

	/* display the code! */
	if (print_reduced)
		im_display();

	/* close file descriptor */
	if (fout)
		fclose(fout);

	return 0;
}

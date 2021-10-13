/* this file operates the assembler.
 it gets files from the command line argument, and runs the assembler on them.
 */

#include "utilities.h"
#include "first_pass.h"
#include "second_pass.h"
#include "output_management.h"

/* initializes a string of name --name-- that includes a given filename with a given extension */
#define initName(name, filename, finish){\
name = malloc(strlen(filename)+strlen(finish)+1);\
strcpy(name, filename);\
strcat(name, finish);\
}

/* freeing of variables in file iterations at main*/
#define freeiter() {\
free(name_as);\
free(name_obj);\
free(name_entry);\
free(name_externals);\
freetable(symbol_table);\
}

#define MAX_EXTENSION 4 /* including '\0'*/


int main(int argc, const char *argv[]) {
	int fileidx;
	char *name_as, *name_obj, *name_entry, *name_externals;
	name_as = name_obj = name_entry = name_externals = NULL;


	/* print error message once no file is given as cmd argument */
	if (argc < 2) {
		printf("Error: assembly files have not been given. Put them as command line arguments to the program (without \".as\" extension).\n");
		exit(0);
	}

	/* go through each file, and try to produce output files for it*/
	for (fileidx = 1; fileidx < argc; ++fileidx) {
		FILE *asfile;
		int dc, ic; /* data & instruction counter- points at the next free spot in the data & instruction image arrays*/
		labelNode symbol_table = NULL;
		MemCell instruct_img[MAX_INSTRUCT_IMG] = {0}, data_img[MAX_DATA_IMG] = {0};

		name_as = malloc(strlen(argv[fileidx]) + MAX_EXTENSION + 1); /* include '\0' (+1) */
		strcpy(name_as, argv[fileidx]); /* now contains the filename without extension .as*/

		asfile = fopen(strcat(name_as, ".as"), "r");

		/* create strings holding the output filenames*/
		/* these will be dynamically allocated & freed later*/
		initName(name_obj, argv[fileidx], ".obj")
		initName(name_externals, argv[fileidx], ".ext")
		initName(name_entry, argv[fileidx], ".ent")

		if (!asfile) {
			printf("Error: cant open file \"%s\". Moving to the next file.\n", name_as);
			freeiter()
			continue;
		}

		putchar('\n');
		printf("Processing file \"%s\"...\n", name_as);
		/* run first pass on the current file, updating dc*/
		dc = firstPas(asfile, data_img, instruct_img, &symbol_table);

		if (dc == ERROR) { /* then no need to create output files. move to the next file*/
			freeiter()
			continue;
		}

		/* first pass is valid. run the second one*/
		rewind(asfile); /* go back to the beggining of the file */
		/* run second pass, updating ic*/
		ic = secondPas(asfile, instruct_img, &symbol_table);
		if (ic == ERROR) {
			freeiter()
			continue;
		}

		fclose(asfile); /* we no longer need it */

		/* now we have all we need for output files: data & instruction memory, and a valid symbol table*/



		printf("Successfully compiled file. \nGenerating output files \"%s\", \"%s\" & \"%s\"...\n", name_obj,
			   name_entry, name_externals);

		createOutputFiles(data_img, instruct_img, symbol_table, ic, dc, name_obj, name_entry, name_externals);

		printf("Files generated successfully.\n");

		/* free everything allocated in this loop */
		freeiter()


	}
	putchar('\n');

	return 0;
}

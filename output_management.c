/* this file contains functions related to making the output files from all data collected during first & second pass. */

#include "output_management.h"

#define C_NUM_CHAR_ADDR

/* gets a number and puts it's octal representation in a given buffer.
 it puts exactly 5 characters of the representation, padding zeros to the left of it if there
 is less than 5 characters.
 the buffer must be at least 6 characters long (including '\0' place)*/
void putBase8(short num, char *buf) {
	char cp[12] = {0}; /* large enaugh to hold the biggest number in octal*/
	char *ret = cp;

	sprintf(cp, "%5.5o", num);
	if (strlen(cp) > 5) { /* if we have more than 5 characters- take the 5 most right ones*/
		ret += strlen(cp) - 5;
	}
	strcpy(buf, ret);
}

/* gets a data image, an instruction image, a symbol table, the ic and dc, and file names of the object, entry and external files that the user wishes to generate through this function.
 it creates three files:
 -object file: contains a list of  adresses and their content in base 8, taken from the instruction image and then from the data image.
 -entry file: containing a list of labels declared as ENTRY and their value in the symbol table.
 -extern file: containing a list of labels declared as EXTERN and their value in the symbol table.*/
void
createOutputFiles(MemCell data_img[], MemCell instruct_img[], labelNode symbol_table, int ic, int dc, char *name_obj,
				  char *name_entry, char *name_externals) {
	int hasentries, hasexterns; /* signals whether to create these files or not*/
	int i;
	labelNode curr;
	FILE *obj, *ent, *ext;

	hasentries = hasexterns = 0;

	/* create .obj file */
	obj = fopen(name_obj, "w");
	if (!obj) {
		printf("Error creating file.\n");
		return;
	}
	/* write title of it */
	fprintf(obj, "%d %d\n", ic, dc);
	for (i = 0; i < ic; ++i) { /* go through each word in instruction image*/
		int addr = i + C_STARTING_MEM;
		char content[5 + 1] = {0};
		putBase8(instruct_img[i],
				 content); /* get the content of the current memory cell in base 8 and store it at content*/
		fprintf(obj, "%04d %s\n", addr, content);
	}

	/* now do the same with data image, with adresses starting right after the last adress written to the file */
	for (i = 0; i < dc; ++i) {
		int addr = ic + C_STARTING_MEM + i;
		char content[6] = {0};
		putBase8(data_img[i], content);
		fprintf(obj, "%04d %s\n", addr, content);
	}
	fclose(obj);

	/* write .ent file */
	ent = fopen(name_entry, "w");
	if (!ent) {
		printf("Error creating file.\n");
		return;
	}

	curr = symbol_table;
	while (curr) {
		if (curr->type == TYPE_ENTRY) {
			hasentries = 1;
			fprintf(ent, "%s %04d\n", curr->name, curr->val);
		}
		curr = curr->next;
	}
	fclose(ent);
	if (!hasentries) { /* dont produce the file if no entry label exists */
		remove(name_entry);
	}

	/* write .ext file */
	ext = fopen(name_externals, "w");
	if (!ext) {
		printf("Error creating file.\n");
		return;
	}

	curr = symbol_table;
	while (curr) {
		if (curr->type == TYPE_EXTERNAL) {
			hasexterns = 1;
			fprintf(ent, "%s %04d\n", curr->name, curr->val);
		}
		curr = curr->next;
	}
	fclose(ext);
	if (!hasexterns) {
		/* then delete the file */
		remove(name_externals);
	}

}


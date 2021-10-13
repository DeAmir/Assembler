#ifndef _OUTPUTMANAGE_H_
#define _OUTPUTMANAGE_H_

#include "mem_cell.h"
#include <stdio.h>
#include <stdlib.h>
#include "label_table.h"

/* gets a data image, an instruction image, a symbol table, the ic and dc, and file names of the object, entry and external files that the user wishes to generate through this function.*/
void createOutputFiles(MemCell data_img[], MemCell instruct_img[], labelNode symtable, int ic, int dc, char *name_obj,
					   char *name_entry, char *name_externals);

#endif /* _OUTPUTMANAGE_H_ */

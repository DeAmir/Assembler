#ifndef _SECOND_PAS_H_
#define _SECOND_PAS_H_

#include <stdio.h>
#include "mem_cell.h"
#include "label_table.h"

/* gets an assemble file, instruction image and symbol table and does second passage on them. */
int secondPas(FILE *asfile, MemCell instruct_img[], labelNode *symbol_table);

#endif /* _SECOND_PAS_H_ */

#ifndef  _FIRSTPAS_H_
#define _FIRSTPAS_H_

#include <stdio.h>
#include "const_tables.h"
#include "mem_cell.h"
#include "label_table.h"

/* runs a first pass on a given file, data immage, instruction image & symbol table */
int firstPas(FILE *asfile, MemCell data_img[], MemCell instruct_img[], labelNode *symbol_table);


/* gets a register string, in the format "*r1" or "r1" (1 could be 1,2,..,8) and returns the number of that register (as an int) */
#define getRegNum(regstr) ((regstr[0] != '*')?(regstr[1] - '0'):(regstr[2]-'0'))

#endif /* _FIRSTPAS_H_ */

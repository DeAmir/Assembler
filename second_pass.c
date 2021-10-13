/* this file contains all the assembler needs to run the second pass (on a file it chooses)*/

#include "second_pass.h"
#include "utilities.h"

extern int linenum;

/* gets a line containing arguments of operation, operation name, instruction image, the next free index in it and a symbol table. codes all
 arguments that are adressed by ADDRM_DIR. returns ERROR on error. otherwise returns the free index in the instruction image after all changes to it.
 the function assumes the input is writtenly valid - it wont check for such issues. */
int dealWithOper2(char *ln_after_oper, char *oper, char *opt_label, MemCell image[], int freeidx, labelNode *symtable) {
	int numargs = numArgs2Op(oper);

	freeidx++; /* skip first word of operation */

	if (numargs >= 1) { /* operate only on the ones that do take operands */
		int addrM1, addrM2;
		char arg1[C_MAXLABEL], arg2[C_MAXLABEL];

		addrM1 = addrM2 = 0;

		getArg(ln_after_oper, arg1);
		addrM1 = addrMethod(arg1);

		if (numargs == 2) {
			/* then it's 2 arguments. fetch info about the second operand*/
			ln_after_oper += strlen(arg1);
			ln_after_oper = cutWhite(ln_after_oper) + 1;
			getArg(ln_after_oper, arg2);

			addrM2 = addrMethod(arg2);
		}

		if ((addrM1 == ADDRM_REG_DIR || addrM1 == ADDRM_REG_INDIR) &&
			(addrM2 == ADDRM_REG_DIR || addrM2 == ADDRM_REG_INDIR)) {
			/* then one additional cell has been taken in memory */
			freeidx++;
		} else {
			if (numargs >= 1) {
				if (addrM1 == ADDRM_DIR) { /* label- fill the missing data in memory */
					labelNode node1 = getLabel(*symtable, arg1);
					int isextern, addr;
					if (node1 == NULL) {
						prerr("label isnt declared")
						return ERROR;
					}
					isextern = (node1->type) == TYPE_EXTERNAL;
					addr = node1->val;
					image[freeidx] = additionalDir(addr, isextern); /* fill place in memory */

					if (node1->type == TYPE_EXTERNAL) {
						if (node1->val == 0) { /* then no need to add a new one, change the existing one */
							node1->val = freeidx + C_STARTING_MEM;
						} else { /* add a copy of it with the value */
							addLabelNoCheck(symtable, node1->name, freeidx + C_STARTING_MEM, LOC_NONE, TYPE_EXTERNAL);
						}
					}
				}
				freeidx++; /* first operand takes a single word */

				if (numargs == 2) {
					/* code the next second operand */
					if (addrM2 == ADDRM_DIR) {
						int addr, isextern;
						labelNode node2 = getLabel(*symtable, arg2);
						if (node2 == NULL) {
							prerr("label isnt declared")
							return ERROR;
						}
						addr = node2->val;
						isextern = (node2->type) == TYPE_EXTERNAL;
						image[freeidx] = additionalDir(addr, isextern);

						if (node2->type == TYPE_EXTERNAL) {
							/* add a copy of it with the value */
							if (node2->val == 0) { /* then no need to add a new one*/
								node2->val = freeidx + C_STARTING_MEM;
							} else {
								addLabelNoCheck(symtable, node2->name, freeidx + C_STARTING_MEM, LOC_NONE,
												TYPE_EXTERNAL);
							}
						}
					}
					freeidx++; /* takes a single word */
				}
			}
		}
	}
	return freeidx;
}

/* runs second pass on a file. returns ERROR on fail, and the ic on success */
int secondPas(FILE *asfile, MemCell instruct_img[], labelNode *symbol_table) {
	int successful = TRUE;
	int ic = 0;
	char line[C_MAXLINE] = {0};

	linenum = 0;

	while (readLine(line, asfile) != EOF) {
		char label[C_MAXLABEL] = {0}, *afterlabel, op_or_instruct[C_MAX_OP_AND_INSTR];
		linenum++;


		if (*line == '\0' || line[0] == ';') { /* skip empty, only whitespace, and comment lines.*/
			continue;
		}

		afterlabel = containsLabel(line, label);

		/* fetch operation or instruction into buffer*/
		getOpInstruct(afterlabel, op_or_instruct);

		if (strcmp(op_or_instruct, I_STRING) == 0 || strcmp(op_or_instruct, I_DATA) == 0 ||
			strcmp(op_or_instruct, I_EXTERN) == 0) { /* skip these lines - delt with them in the first pass */
			continue;
		}
		if (strcmp(op_or_instruct, I_ENTRY) == 0) {
			/* no errors had been checked here during the first pass. need to check here*/
			char addlabel[C_MAXLABEL] = {0}; /* label after entry command*/
			char *afterentry = cutWhite(afterlabel + strlen(I_ENTRY));
			char *templab = malloc(C_MAXLABEL + 2);
			char *ptr = templab; /* save reference for freeing*/
			labelNode lab;

			if (!afterentry || strlen(afterentry) == 0) {
				prerr(".entry must take an operand")
				return ERROR;
			}

			strcpy(templab, afterentry);
			if (containsWhite(templab)) {
				prerr("label cannot contain whitespaces")
				successful = 0;
				free(ptr);
				continue;
			}
			templab = strcat(templab, ":c"); /* add for checking with containsLabel*/
			if (containsLabel(templab, addlabel) == (char *) ERROR) {
				successful = 0;
				free(ptr);
				continue;
			}
			/* its a valid label then*/
			/* an entry label must be declared before */
			lab = getLabel(*symbol_table, templab);
			if (lab == NULL) {
				prerr("label has not been defined")
				successful = 0;
				free(ptr);
				continue;
			}
			/* add "entry" type to the label in the symbol label (only if it's not external) */
			if (lab->type != TYPE_EXTERNAL) {
				lab->type = TYPE_ENTRY;
			} else { /* if external- add another label witt value 0*/
				addLabelNoCheck(symbol_table, lab->name, 0, lab->location, TYPE_ENTRY);
			}
			free(ptr);
		} else { /* then its an operation */
			char *afterop = afterlabel + strlen(op_or_instruct);
			afterop = cutWhite(afterop);

			ic = dealWithOper2(afterop, op_or_instruct, label, instruct_img, ic, symbol_table);
			if (ic == ERROR) {
				successful = 0;
			}
		}

	}
	return (!successful) ? ERROR : ic;

}

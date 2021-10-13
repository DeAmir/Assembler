/* this file contains everything the assemble needs to run the first pass on a given file. */

#include "first_pass.h"
#include "utilities.h"

#define BITS_CELL 15 /* each cell has 15 bits*/

/* counts the current line being read */
int linenum = 0;


int dealWithInstruct(char *line_after_instruct, char *instruct, char *optional_label, MemCell image[], int freeidx,
					 labelNode *symtable);

int dealWithOper(char *line_after_oper, char *oper, char *optional_label, MemCell image[], int freeidx,
				 labelNode *symtable);

void incDataLabelsBy(labelNode table, int add);


/* does first run on a given assembly file. returns ERROR on fail and data counter on sucess */
int firstPas(FILE *asfile, MemCell data_img[], MemCell instruct_img[], labelNode *symbol_table) {
	int successful = TRUE;
	char line[C_MAXLINE] = {0};
	int ic, dc; /* instruction counter & data counter */
	char op_or_instruct[C_MAX_OP_AND_INSTR], label[C_MAXLABEL];

	ic = dc = 0;

	linenum = 0;

	while (readLine(line, asfile) != EOF) { /* get each line without start & end whitespaces */
		char *afterlabel, *after_opinstruct;
		int actiontype;

		memset(op_or_instruct, 0, C_MAX_OP_AND_INSTR);
		memset(label, 0, C_MAXLABEL);

		linenum++;


		if (*line == '\0' || line[0] == ';') { /* skip empty, only whitespace, and comment lines */
			continue;
		}

		afterlabel = containsLabel(line, label);

		if (afterlabel == (char *) ERROR) {
			successful = 0;
			continue; /* skip to the next line*/
		}


		actiontype = getOpInstruct(afterlabel, op_or_instruct);

		if (actiontype == ERROR) {
			successful = 0;
			continue;
		}

		after_opinstruct = afterlabel + strlen(op_or_instruct);
		after_opinstruct = cutWhite(after_opinstruct);

		if (actiontype == INST) {
			int newdc = dealWithInstruct(after_opinstruct, op_or_instruct, label, data_img, dc, symbol_table);
			if (newdc == ERROR) {
				successful = 0;
				continue;
			}
			dc = newdc;
		} else if (actiontype == OPER) {
			int newic;
			newic = dealWithOper(after_opinstruct, op_or_instruct, label, instruct_img, ic, symbol_table);
			if (newic == ERROR) {
				successful = 0;
				continue;
			}
			ic = newic;
		}

	}
	/* increase all labels marked with data attribute (for second pass) */
	incDataLabelsBy(*symbol_table, C_STARTING_MEM + ic);

	/* return dc on success */
	return (!successful) ? ERROR : dc;
}

/* gets a line that contains arguments of an instruction, gets an instruction name, a label name, data image, and symbol table.
 codes the right memory based on these into the image.
 returns the new dc value. */
int dealWithInstruct(char *ln_after_instruct, char *instruct, char *opt_label, MemCell image[], int dc,
					 labelNode *symtable) {

	if (!hasExtraText(ln_after_instruct)) {
		prerr("instruction must take an operand")
		return ERROR;
	}

	if (strcmp(instruct, I_DATA) == 0 || strcmp(instruct, I_STRING) == 0) { /* if the instruction is .string or .data */
		if (tableContainsLabel(*symtable, opt_label)) {
			prerr("label already declared")
			return ERROR;
		}
		if (opt_label && opt_label[0] != '\0') {
			/* then there is a label*/
			/* insert label as "data" into symbol table */
			addLabel(symtable, opt_label, dc, LOC_DATA, TYPE_NONE);
		}
		if (strcmp(instruct, I_STRING) == 0) {
			int i;
			char *string = getString(ln_after_instruct);
			if (string == (char *) ERROR) { /* if the parameter string is invalid */
				return ERROR;
			}
			/* now it is a valid string. insert it into the image, character by character */
			for (i = 0; i < strlen(string); i++) {
				image[dc] = string[i];
				dc++;
			}
			image[dc++] = 0;
			free(string);
		} else { /* then its .data */
			short num;
			char *numst, *lncp;
			unsigned mask15 = 0, i;

			/* create an int that has all first 15 bits turned on, and others off*/
			for (i = 0; i < BITS_CELL; ++i) {
				mask15 <<= 1;
				mask15 |= 1;
			}

			lncp = malloc(strlen(ln_after_instruct) + 1);
			strcpy(lncp, ln_after_instruct); /* copy it so we dont mess up the original with strtok */


			if (lncp[strlen(lncp) - 1] == ',') {
				prerr("invalid comma")
				return ERROR;
			}

			numst = strtok(lncp, ",");

			while (numst) {
				int sign;
				numst = cutWhite(numst);
				if (!isValidNumber(numst)) {
					free(lncp);
					return ERROR;
				}
				/* then valid number. insert it */
				/* move the sign bit from the last one to the 15th bit*/
				num = atoi(numst);
				sign = (num & (1 << (sizeof(short) * 8 - 1))) ? 1 : 0; /* get bit of sign*/
				num &= mask15; /* turn off all bits after the 15th*/
				num |= sign << (BITS_CELL - 1); /* put the sign bit in place- at the 15th bit*/
				/* now the number is ready. add it*/
				image[dc++] = num;
				/* move to the next number */
				numst = strtok(NULL, ",");
			}
			free(lncp);
		}
	} else { /* then we deal with a label*/
		char *checklabel; /* ":c" + '\0'*/
		if (!ln_after_instruct) {
			prerr("no label after instruction")
			return ERROR;
		}
		checklabel = malloc(strlen(ln_after_instruct + 3));
		strcpy(checklabel, ln_after_instruct);
		strcat(checklabel, ":c");
		if (containsLabel(checklabel, ln_after_instruct) == (char *) ERROR) {
			free(checklabel);
			return ERROR;
		}
		if (strcmp(instruct, I_EXTERN) == 0) {
			labelNode temp;

			/* then ln_after_instruct is a label */
			/* if label already declared - thats an error */


			free(checklabel);

			if ((temp = (getLabel(*symtable, ln_after_instruct))) && temp->type != TYPE_EXTERNAL) {
				prerr("a declared label in file cannot be external")
				return ERROR;
			} else {
				/* insert argument as external label with value 0 into the symbol table*/
				addLabel(symtable, ln_after_instruct, 0, LOC_NONE, TYPE_EXTERNAL);
			}

		} else { /* then its .entry. check if has an operand only*/
			if (!ln_after_instruct) {
				prerr("no label after .entry")
				return ERROR;
			}
		}
	}
	return dc;
}

/*  gets a line that contains arguments of an operation, gets an operstion name, a label name, data image, and symbol table.
 codes the memory it takes into the image. returns the new ic value. */
int dealWithOper(char *ln_after_oper, char *oper, char *opt_label, MemCell image[], int ic, labelNode *symtable) {
	int numargs;
	/* firstly deal with label */
	if (opt_label && *opt_label != '\0') {
		/* then a label exists.*/
		if (tableContainsLabel(*symtable, opt_label)) {
			prerr("label has already been declared")
			return ERROR;
		}
		/* insert it as code (location attribute), with loading index from 100 (C_STARTING_MEM) */
		addLabel(symtable, opt_label, ic + C_STARTING_MEM, LOC_CODE, TYPE_NONE);
	}

	/* move after the operation*/
	numargs = numArgs2Op(oper); /* get number of arguments the operation takes */
	if (numargs == 0) {
		if (hasExtraText(ln_after_oper)) {
			prerr("extra text after operation");
		}
		/* source & destination adressing methods are 0- we have no arguments at all */
		image[ic++] = firWord(0, 0, getOpCode(oper), numargs);
	} else if (numargs >= 1) {
		int addrM1, addrM2, code;
		char arg1[C_MAXLABEL], arg2[C_MAXLABEL];

		addrM1 = addrM2 = 0;

		if (hasDobleCommas(ln_after_oper)) {
			prerr("contains two consecutive commas (,,)")
			return ERROR;
		}

		/* fetch first argument into arg1*/
		code = getArg(ln_after_oper, arg1);
		if (code == ERROR) {
			return ERROR;
		}
		/* get adressing method*/
		addrM1 = addrMethod(arg1);

		if (numargs == 1) {
			if (hasExtraText(ln_after_oper + strlen(arg1))) {
				prerr("extra text after command");
				return ERROR;
			}
			/* check iif the adressing method fits with the operation */
			if (opAdrrValid(oper, -1, addrM1) == FALSE) {
				return ERROR;
			}
			/* then the argument if valid. */

		} else { /* then it's 2 arguments*/
			/* skip the first argument */
			ln_after_oper += strlen(arg1);
			ln_after_oper = cutWhite(ln_after_oper);

			if (ln_after_oper && ln_after_oper[0] == ',') {
				ln_after_oper++;
			}
			/* fetch the second argument into arg2 */
			code = getArg(ln_after_oper, arg2);
			if (code == ERROR) {
				return ERROR;
			}
			/* check if addressing methods are valid for operation*/
			addrM2 = addrMethod(arg2);
			if (!opAdrrValid(oper, addrM1, addrM2)) {
				return ERROR;
			}
			ln_after_oper = cutWhite(ln_after_oper);
			if (hasExtraText(ln_after_oper + strlen(arg2))) {
				prerr("extra text after command");
				return ERROR;
			}

			/* then valid two arguments. */

		}
		/* insert the first word */
		image[ic++] = firWord(addrM1, addrM2, getOpCode(oper), numargs);

		/* check if we have two register arguments*/
		if ((addrM1 == ADDRM_REG_DIR || addrM1 == ADDRM_REG_INDIR) &&
			(addrM2 == ADDRM_REG_DIR || addrM2 == ADDRM_REG_INDIR)) {
			/* then only one additional cell will be added */
			int reg1num = getRegNum(arg1);
			int reg2num = getRegNum(arg2);
			image[ic++] = additionalRegComb(reg1num, reg2num);
		} else {
			if (numargs >= 1) {
				/* arg1 is destination argument if numargs is 1*/
				int isarg1dest = (numargs == 1) ? 1 : 0;
				/* add the first operands cell */
				if (addrM1 == ADDRM_IMM) {
					/* then insert the number it represents */
					int num;
					char *numst = arg1 + 1; /* skip the '#' */
					if (!isValidNumber(numst)) {
						return ERROR;
					}

					num = atoi(numst);
					image[ic++] = additionalImm(num);
				} else if (addrM1 == ADDRM_REG_INDIR) {
					int regnum = getRegNum(arg1);
					image[ic++] = additionalReg(isarg1dest, regnum);
				} else if (addrM1 == ADDRM_REG_DIR) {
					int regnum = getRegNum(arg1);
					image[ic++] = additionalReg(isarg1dest, regnum);
				} else { /* then its ADDR_DIR */
					/* save space for later- ADDR_DIR will be handled in the second passage */
					image[ic++] = 0;
				}

				/* first operand has been coded. */

				if (numargs == 2) {
					/* code the second one */
					if (addrM2 == ADDRM_IMM) {
						/* then insert the number */
						int num;
						char *numst = arg2 + 1; /* skip the '#' */
						if (!isValidNumber(numst)) {
							return ERROR;
						}
						num = atoi(numst);

						image[ic++] = additionalImm(num);
					} else if (addrM2 == ADDRM_REG_DIR) {
						int regnum = getRegNum(arg2);
						image[ic++] = additionalReg(1, regnum);
					} else if (addrM2 == ADDRM_REG_INDIR) {
						int regnum = getRegNum(arg2);
						image[ic++] = additionalReg(1, regnum);
					} else { /* then its ADDR_DIR */
						/* save space for later- ADDR_DIR will be handled in the second passage*/
						image[ic++] = 0;

					}
				}
			}
		}
	}
	/* return the changed counter */
	return ic;
}

/* gets a list (of type labelNode) and an int add.
 goes through the list and increases every label with attribute "data" by add.
 returns nothing.*/
void incDataLabelsBy(labelNode table, int add) {
	labelNode curr = table;
	while (curr) {
		if (curr->location == LOC_DATA) { /* then update its value*/
			curr->val += add;
		}

		curr = curr->next;
	}
}

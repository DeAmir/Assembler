#include "const_tables.h"

/* every array is sorted by opcode- low to high */

char *opgroup_2[C_NUM_2_OPER] = {O_MOV, O_CMP, O_ADD, O_SUB, O_LEA};
char *opgroup_1[C_NUM_1_OPER] = {O_CLR, O_NOT, O_INC, O_DEC, O_JMP, O_BNE, O_RED, O_PRN, O_JSR};
char *opgroup_0[C_NUM_0_OPER] = {O_RTS, O_STOP};
char *regs[C_NUM_REGS] = {R_0, R_1, R_2, R_3, R_4, R_5, R_6, R_7};

char *instructs[C_NUM_INSTRUCTS] = {I_STRING, I_DATA, I_ENTRY, I_EXTERN};

/* gets an array of strings and searches for a given string inside it. returns 1+idx of it if found, otherwise 0 (FALSE).*/
int strInArr(char **listofstring, char *search4, int len) {
	int idx;
	for (idx = 0; idx < len; ++idx) {
		if (strcmp(listofstring[idx], search4) == 0) {
			return idx + 1;
		}
	}
	return FALSE;
}

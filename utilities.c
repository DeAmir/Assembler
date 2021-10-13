/* this file contains many "helper functions" that will be used in other parts of the program. */

#include "utilities.h"


extern int linenum;

extern char *opgroup_2[], *opgroup_1[], *opgroup_0[], *regs[], *instructs[];

/* returns the number of appearances of character c in  string str */
int numCharInStr(char *str, char c) {
	int count = 0;
	while (str && *str) {
		if ((*str) == c) {
			count++;
		}
		str++;
	}
	return count;
}

/* gets a string and returns TRUE if it only contains digits and alphabetic letters. otherwise, FALSE.*/
int containsOnlyDigitsLetters(char *str) {
	while (*str != '\0') {
		if (!isalpha(*str) && !isdigit(*str)) {
			return FALSE;
		}
		++str;
	}
	return TRUE;
}

/* gets a string and returns TRUE if it is: a register, an instruction, and operation name. otherwise FALSE. */
int isSavedName(char *str) {

	if (isReg(str) || isInstruct(str) || isOper0(str) || isOper1(str) || isOper2(str)) {
		return TRUE;
	}
	return FALSE;

}


/* returns FALSE when num isn't valid or if pointer is null or if a string of length 0. returns TRUE on success.*/
int
isValidNumber(char *str) {
	if (!str || str[0] == '\0') {
		prerr("no number given between commas")
		return FALSE;
	}

	/* skip '+' or '-' if exists */
	if (str[0] == '+' || str[0] == '-') {
		str++;
	}
	if (!(*str)) {/* it is "+" or "-" - no number in here!*/
		prerr("'+' or '-' aren't valid numbers")
		return FALSE;
	}

	while (*str) {
		if (!isdigit(*str)) {
			prerr("a number contains a non-digit letter");
			return FALSE;
		}
		str++;
	}

	return TRUE;

}

/* gets a string that contains a string. returns ERROR if the inner string is unvalid. returns the inner string for a valid string.*/
char *getString(char *str) {
	int stlen, numofquotes, i, len;
	char *ret;

	if (!str) {
		return (char *) ERROR;
	}
	stlen = strlen(str);
	numofquotes = numCharInStr(str, '"');

	if (numofquotes > MAX_QUOTES) {
		prerr("string not valid- must have exactly 2 quotes");
		return (char *) ERROR;
	}
	if (str[0] != '"' || str[stlen - 1] != '"' || numofquotes < 2) {
		prerr("string not valid- missing quotes in the beggining or end of the string");
		return (char *) ERROR;
	}
	/* now it is a valid string */
	/* switch back any '\n' with ':' */
	for (i = 0; i < stlen; ++i) {
		if (str[i] == C_COLON_IN_STRING) {
			str[i] = ':';
		}
	}
	len = stlen - 2;/* because we dont include two quotes */
	ret = (char *) malloc(len + 1); /* +1 for the '\0'*/
	strncpy(ret, str + 1, len); /* +1 to skip the first quote*/
	ret[len + 1] = '\0'; /* remove the last  quote */

	return ret;
}

/* reads a line from a given file, removes before & after whispaces, and puts it into a buffer. it switches any ':' character inside a string (e.g "hey:brother") with '\n' (e.g "hey\nbrother").
 returns EOF on end-of-file, and TRUE on success.*/
int readLine(char *dest, FILE *file) {
	char *ptr, *templine = (char *) malloc(C_MAXLINE), *colon;

	ptr = fgets(templine, C_MAXLINE, file);
	if (ptr == NULL) {
		return EOF;
	}
	ptr = cutWhite(ptr);
	colon = ptr;
	/* switch strstr*/
	while ((colon = strstr(colon, ":")) != NULL) {
		int idx, numBef, numAfter;
		idx = colon - ptr;
		ptr[idx] = '\0'; /* switch ':' with '\0'*/
		numBef = numCharInStr(ptr, '\"');
		ptr[idx] = ':'; /* switch back */
		numAfter = numCharInStr(colon + 1, '\"');
		if (numBef % 2 != 0 && numAfter % 2 != 0) { /* if we are in a string */
			ptr[idx] = C_COLON_IN_STRING;
		}
		colon++;
	}
	strcpy(dest, ptr);
	free(templine);
	return TRUE;

}

/* gets a string and removes all whitespaces from it's end.
 returns a pointer to the first cell in the string that doesn't contain whitespaces. */
char *cutWhite(char *s) {
	int i;
	if (!s || !(*s) || (*s) == EOF) {
		return NULL;
	}

	while (isspace(*s)) { /* skip starting whitespaces*/
		++s;
	}

	/* remove ending whitespaces*/
	for (i = strlen(s) - 1; i >= 0; --i) {
		if (isspace(s[i])) {
			s[i] = '\0';
		} else {
			break;
		}
	}

	return s;

}


/* checks if a line of command contains a label in the beggining. if so, it puts that label at putlabelat.
 when a valid label is present, returns a pointer to the part next to the lebel (without the ':').
 when not, returns ERROR on error and the string itself when no label is present.*/
char *containsLabel(char *line, char *putlabelat) {
	char *label, *ret;

	int num = numCharInStr(line, ':');
	if (num > 1) {
		prerr("more than one label in a single command");
		return (char *) ERROR;
	} else if (num < 1) {
		return line; /* valid- no label present*/
	}
	/* now it contains only one ':' */
	label = strtok(line, ":");

	/* labels like "hey :" are invalid */
	if (isspace(label[strlen(label) - 1])) {
		prerr("there must be no whitespaces between ':' and the label")
		return (char *) ERROR;
	}

	label = cutWhite(label);


	/* check if label is valid */
	if (label[0] == '\0') {
		prerr("no label has been given before ':' character");
		return (char *) ERROR;
	} else if (!isalpha(label[0])) {
		prerr("label name starts with a non-alphabetic character");
		return (char *) ERROR;
	} else if (!containsOnlyDigitsLetters(label)) {
		prerr("label name contains non-digit or non-alphabetic character or contains a whitespace inside it");
		return (char *) ERROR;
	} else if (strlen(label) > C_MAXLABEL - 1) { /* without '\0'*/
		prerr("label is too long. Maximum length is 31 letters");
		return (char *) ERROR;
	} else if (isSavedName(label)) {
		prerr("a saved word (register name, operation name, instruction name) can't be a label");
		return (char *) ERROR;
	}

	/* now it's valid. */
	strcpy(putlabelat, label);

	/* get the other part of the line, without the LABEL and the ':'*/
	ret = cutWhite(strtok(NULL, ":"));


	return ret;
}

/* gets a line, gets the first word of it: if it is a valid operation name, return OPER and puts the name in putat, if it's a valid instruction name- returns INST and puts the name in putat. if there are errors, returns ERROR and prints error message.
 assumes the line doesnt contain whitespaces in the beggining or end.*/
int getOpInstruct(char *line, char *putat) {
	char fir_word[C_MAX_OP_AND_INSTR] = {0};
	char *tmp = line;
	int wordlen = 0;
	while (*tmp != '\0' && !isspace(*tmp)) {
		tmp++;
		wordlen++;
	}
	if (wordlen > C_MAX_OP_AND_INSTR) {
		prerr("operation or instruction is too long")
		return ERROR;
	}
	sscanf(line, "%s ", fir_word);
	if (isInstruct(fir_word)) {
		strcpy(putat, fir_word);
		return INST;
	} else if (isOper0(fir_word) ||
			   isOper1(fir_word) || isOper2(fir_word)) {
		strcpy(putat, fir_word);
		return OPER;
	}
	prerr("unknown operation or instruction")
	return ERROR;
}

/* gets an operation name and returns the number of operands it takes. assummes the operation is a valid operation. */
int numArgs2Op(char *op) {
	if (isOper0(op)) {
		return 0;
	} else if (isOper1(op)) {
		return 1;
	} else {
		return 2;
	}
}


/* gets a string and returns TRUE if it contains a whitespace character, otherwise false. */
int containsWhite(char *str) {
	while (*str) {
		if (isspace(*str)) {
			return TRUE;
		}
		str++;
	}
	return FALSE;
}

/* gets the first argument in an operation line. if exists, returns TRUE and puts the argument in storeat. if not, returns ERROR.*/
int getArg(char *fromln, char *storeat) {
	char *line = malloc(C_MAXLINE); /* copy the line for use of strtok*/
	char *arg;

	if (!fromln) {
		prerr("missing argument")
		return ERROR;
	}
	if (fromln[0] == ',') {
		prerr("invalid comma before argument")
		return ERROR;
	}

	strcpy(line, fromln);
	arg = strtok(line, ",");
	arg = cutWhite(arg);


	if (arg == NULL) {
		prerr("missing an operation argument");
		return ERROR;
	} else if (containsWhite(arg)) {
		prerr("an operation argument cannot contain whitespaces")
		return ERROR;
	} else if (strlen(arg) > C_MAXLABEL) {
		prerr("argument name is too big")
		return ERROR;
	}
	/* valid argument*/
	strcpy(storeat, arg);
	free(line);

	return TRUE;

}


/* gets an argument as string and returns the adressing method of it*/
int addrMethod(char *arg) {
	if (arg[0] == '#') {
		return ADDRM_IMM;
	} else if (arg[0] == '*') {
		return ADDRM_REG_INDIR;
	} else if (isReg(arg)) {
		return ADDRM_REG_DIR;
	} else {
		return ADDRM_DIR;
	}
}

/* returns opcode for a given operation*/
int getOpCode(char *op) {
	int idxpls1 = isOper2(op);
	if (idxpls1) { /* if op takes 2 arguments*/
		return idxpls1 - 1;
	}
	idxpls1 = isOper1(op);
	if (idxpls1) {
		return (idxpls1 - 1) + C_NUM_2_OPER;
	}
	/* then it gets no params: */
	return (isOper0(op) - 1) + C_NUM_1_OPER + C_NUM_2_OPER;
}

/* checks if an adressing method for two or one argument for an operation is valid. TRUE if it is, otherwise FALSE. prints error messages. */
int opAdrrValid(char *oper, int sourceAddrM, int destAddrM) {
	char *srcValid[C_NUM_2_OPER] = {"0123", "0123", "0123", "0123", "1"};
	char *destValid[
			C_NUM_2_OPER + C_NUM_1_OPER] = {"123", "0123", "123", "123", "123", "123", "123", "123", "123", "12", "12",
											"123", "0123", "12"};
	/* ordered so at array index opcode are the valid adressing method for the operation of opcode.*/
	int opcode = getOpCode(oper);
	char destChar = '0' + destAddrM;

	if (isOper1(oper)) {
		/* then only has destination argument */
		if (numCharInStr(destValid[opcode], destChar)) {
			return TRUE;
		} else {
			prerr("invalid addressing method")
			return FALSE;
		}
	} else {
		/* then there are two arguments. check them both*/
		char srcChar = '0' + sourceAddrM;
		if (!numCharInStr(destValid[opcode], destChar)) {
			prerr("invalid adressing method for destination argument")
			return FALSE;
		}
		if (!numCharInStr(srcValid[opcode], srcChar)) {
			prerr("unvalid adressing method for source argument")
			return FALSE;
		}
		return TRUE;
	}
}

#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "const_tables.h"

#define MAX_QUOTES 2 /* maximum quotes in a string */

#define C_MAXLINE 81 /* including '\0' */
#define C_MAXLABEL 32 /* including '\0'*/
#define C_MAX_OP_AND_INSTR 8 /* len of ".string" + '\0' */

#define C_COLON_IN_STRING '\n' /* used in readLine*/

/* type of action in line */
#define OPER 5
#define INST 6

/* adressing methods */
#define ADDRM_IMM 0
#define ADDRM_DIR 1
#define ADDRM_REG_INDIR 2
#define ADDRM_REG_DIR 3
#define ADDRM_UNDEF -1


#define prerr(str) printf("Error at line %d: " str ".\n", linenum);

/* gets a string and returns whether it is a valid integer number */
int isValidNumber(char *);

/* gets a string and returns ERROR if the string doesn't contain a valid string inside it. if it contains a valid string, it returns that valid string. */
char *getString(char *);

/* reads a line from a given file into a buffer. it switches any ':' character inside a string (e.g "hey:brother") with '\n' (e.g "hey\nbrother").
 returns EOF on end-of-file, and TRUE on success.*/
int readLine(char *buff, FILE *);

/* gets a string and removes all whitespaces from it's end.
 returns a pointer to the first cell in the string that doesn't contain whitespaces. */
char *cutWhite(char *s);

/* gets a line (in the form of string) and if that line contains a valid label- it puts it in a given buffer and returns a pointer to the rest of the string (aka without the label).
 when no label is present at all, it just returs the line as is.
 if the line contains an  invalid label, it prints an error message and returns ERROR. */
char *containsLabel(char *line, char *putlabelat);

/* gets a line and checks whether it contains an operation or insturction. returns OPER on operation and INST on instruction. it also puts that operation/instruction in a given buffer.
 iif there are errors it prints an error message and returns ERROR.*/
int getOpInstruct(char *line, char *putat);

/* gets an operation name and returns the number of operands it takes */
int numArgs2Op(char *op);

/* gets a string and returns FALSE if the string is all whitespaces, otherwise TRUE. */
#define hasExtraText(str) ((!(str) || !(cutWhite((str))))?FALSE:TRUE)

/* gets a string and returns TRUE if it contains ",," in it, otherwise FALSE. */
#define hasDobleCommas(str) ( (!(strstr((str),",,"))) ? FALSE : TRUE)


/* gets the first argument from a given line and puts it in a given buffer. returns TRUE on success and ERROR on error. */
int getArg(char *fromln, char *storeat);


/* gets a string and returns TRUE if it contains a whitespace character, otherwise false. */
int containsWhite(char *);

/* gets an argument as string and returns its adressing method */
int addrMethod(char *);

/* gets an operation and two adressing methods- one for source and one for destination, and returns whether the adressing methods are valid for the operations.
 it assumes that the operation name is valid.*/
int opAdrrValid(char *oper, int sourceAddrM, int destAddrM);

/* gets an operation and returns its opcode. assumes that the operation is a valid operation name. */
int getOpCode(char *);

#endif /* _UTILITIES_H_ */

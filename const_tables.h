#ifndef _CONST_TABLES_H
#define _CONST_TABLES_H

#include <string.h>

int strInArr(char **listofstring, char *search4, int len);

extern char *opgroup_2[], *opgroup_1[], *opgroup_0[], *regs[], *instructs[];

/* operations taking two operands */
#define C_NUM_2_OPER 5
#define O_MOV "mov"
#define O_CMP "cmp"
#define O_ADD "add"
#define O_SUB "sub"
#define O_LEA "lea"


/* operations taking one operand */
#define C_NUM_1_OPER 9
#define O_CLR "clr"
#define O_NOT "not"
#define O_INC "inc"
#define O_DEC "dec"
#define O_JMP "jmp"
#define O_BNE "bne"
#define O_RED "red"
#define O_PRN "prn"
#define O_JSR "jsr"

/* operations taking no operand */
#define C_NUM_0_OPER 2
#define O_RTS "rts"
#define O_STOP "stop"

/* registers */
#define C_NUM_REGS 8
#define R_0 "r0"
#define R_1 "r1"
#define R_2 "r2"
#define R_3 "r3"
#define R_4 "r4"
#define R_5 "r5"
#define R_6 "r6"
#define R_7 "r7"

#define TRUE 1
#define FALSE 0
#define ERROR (-1)

/* instructions */
#define C_NUM_INSTRUCTS 4
#define I_STRING ".string"
#define I_DATA ".data"
#define I_ENTRY ".entry"
#define I_EXTERN ".extern"

#define isInstruct(ptr) (strInArr(instructs,(ptr), C_NUM_INSTRUCTS))
#define isOper0(ptr) (strInArr(opgroup_0,(ptr), C_NUM_0_OPER))
#define isOper1(ptr) (strInArr(opgroup_1,(ptr), C_NUM_1_OPER))
#define isOper2(ptr) (strInArr(opgroup_2,(ptr), C_NUM_2_OPER))
#define isReg(ptr) (strInArr(regs, (ptr), C_NUM_REGS))

#endif /* _CONST_TABLES_H */

#ifndef _MEMCELL_H_
#define _MEMCELL_H_

/* each memory cell contains 16 bits, indexed from lsb (0) to msb (15)*/
typedef short MemCell;

#define A 4 /* 100 */
#define R 2 /* 010*/
#define E 1 /* 001*/

#define MAX_DATA_IMG 2048
#define MAX_INSTRUCT_IMG 2048

#define C_STARTING_MEM 100

/* macros for adittional words: each for each adressing method.
 register coding is the same for both direct and indirect, so one macro is created.
 */
#define additionalImm(val) ((((signed int)val)<<3) | A)
#define additionalDir(addr, isExtern) ((isExtern)?E:(((((unsigned int)addr))<<3)|R))
#define additionalReg(isDestArg, regNum)(((isDestArg)?regNum<<3:regNum<<6)|A)

/* used if two operands are adressed by RIndir or Rdir (on that case, they share the same word in memory)*/
#define additionalRegComb(sourRegN, destRegN) ( ((destRegN)<<3) | ((sourRegN)<<6) | A)

/* gets a source & destination addressing method, operation code and number of arguments as parameters. generates the first word to be put in memory, based on them.*/
MemCell firWord(int sourM, int destM, int opcode, int numargs);

#endif /* _MEMCELL_H_ */

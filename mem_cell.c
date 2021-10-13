#include "mem_cell.h"
#include <stdlib.h>

/* gets source adressing method and destination, operation code and number of arguments the operation got.
 it creates the first word to be put in memory, based on those arguments.
 argument.
 the function assumes that both sourM & destM is 0,1,2 or 3. opcode is between 0 and 15.*/
MemCell firWord(int sourM, int destM, int opcode, int numargs) {
	MemCell ret, destMTurned, sourMTurned;
	ret = destMTurned = sourMTurned = 0;

	if (numargs ==
		1) {/* sourM contains the adressing method of the first operation argument. hence, when an operation has only one parameter - that parameter is the destination.*/
		/* therefore sourM contains the adressing method of the destination*/
		destMTurned = 1 << sourM;
	} else if (numargs == 2) {
		sourMTurned = 1 << sourM;
		destMTurned = 1 << destM;
	}

	ret |= A; /* first word always contains A in the field ARE*/
	ret |= (destMTurned) << 3; /* takes bits 3-6*/
	ret |= (sourMTurned) << 7;/* takes bits 7-10*/
	ret |= (opcode) << 11; /* takes bits 11-14*/

	/* the msb is always 0*/

	return ret;
}

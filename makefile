assembler: assembler.o const_tables.o first_pass.o label_table.o mem_cell.o output_management.o second_pass.o utilities.o
	gcc -ansi -Wall -pedantic -g assembler.o const_tables.o first_pass.o label_table.o mem_cell.o output_management.o second_pass.o utilities.o -o assembler

assembler.o: assembler.c utilities.h first_pass.h second_pass.h mem_cell.h output_management.h
	gcc -ansi -Wall -pedantic -c assembler.c -o assembler.o

const_tables.o: const_tables.c const_tables.h
	gcc -ansi -Wall -pedantic -c const_tables.c -o const_tables.o
	
first_pass.o: first_pass.c first_pass.h utilities.h
	gcc -ansi -Wall -pedantic -c first_pass.c -o first_pass.o
	
label_table.o: label_table.c label_table.h
	gcc -ansi -Wall -pedantic -c label_table.c -o label_table.o
	
mem_cell.o: mem_cell.c mem_cell.h
	gcc -ansi -Wall -pedantic -c mem_cell.c -o mem_cell.o
	
output_management.o: output_management.c output_management.h label_table.h
	gcc -ansi -Wall -pedantic -c output_management.c -o output_management.o
	
second_pass.o: second_pass.c second_pass.h label_table.h utilities.h
	gcc -ansi -Wall -pedantic -c second_pass.c -o second_pass.o
	
utilities.o: utilities.c utilities.h const_tables.h
	gcc -ansi -Wall -pedantic -c utilities.c -o utilities.o

#ifndef _LABEL_TABLE_H_
#define _LABEL_TABLE_H_

#include <stdlib.h>
#include <string.h>

#define C_MAXLABEL 32 /* including finishing '\0'*/


/* attributes of a label: */
/* a label is in code segment or data, or non of them (e.g ".extern HEY") */
#define LOC_CODE 1
#define LOC_DATA 2
#define LOC_NONE 3

#define TYPE_EXTERNAL 1
#define TYPE_ENTRY 2
#define TYPE_NONE 3 /*when label is before operation. e.g "A: stop"*/


/* a node in the list. each node represents a label in the code.*/
struct node {
	char *name; /* name of label*/
	int val; /* "pointer" to memory location- contains the current ic or dc*/
	int location; /* code or data (or none) */
	int type; /* external or entry (or none)*/
	struct node *next; /* pointer to the next node in the list*/
};

/* a node pointer */
typedef struct node *labelNode;

/* gets the head of a list and atrributes of label to be added to it. adds the label to the beggining of the list. checks if label exists before adding it (& not addding it if exists)*/
void addLabel(labelNode *headPtr, char *name, int val, int location, int type);

/* gets the head of a list and atrributes of label to be added to it. adds the label to the beggining of the list without checking if it is already in the list. */
void addLabelNoCheck(labelNode *headPtr, char *name, int val, int location, int type);

/* gets the head of a list and a label name as parameters. returns 1 if the list contains a label of the given name. otherwise 0.*/
int tableContainsLabel(labelNode head, char *labname);

/* returns a pointer to a label of a given name in a given list*/
labelNode getLabel(labelNode head, char *name);

/* frees a list from memory */
void freetable(labelNode head);

#endif /* _LABEL_TABLE_H_ */

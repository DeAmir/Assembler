/* this file contains functions to be used on the symbol table. */

#include "label_table.h"

/* gets the head of a list and atrributes of label to be added to it. adds a label that has the given attirbutes to the beggining of the list, without checking if it is already in the list. */
void addLabelNoCheck(labelNode *headPtr, char *name, int val, int location, int type) {
	labelNode head = *headPtr;
	labelNode add = malloc(sizeof(struct node));
	/* set its attributes */
	add->val = val;
	add->location = location;
	add->type = type;
	(add->name) = malloc(C_MAXLABEL);
	strcpy(add->name, name);

	if (head == NULL) { /* when table is empty */
		/* put add as the head*/
		add->next = NULL;
		*headPtr = add;
	} else {
		add->next = head;
		*headPtr = add; /* change the head pointer to point to add */

	}
}

/* gets the head of a list and atrributes of label to be added to it. adds the label to the beggining of the list. checks if label exists before adding it (& not addding it if exists).
 */
void addLabel(labelNode *headPtr, char *name, int val, int location, int type) {
	if (tableContainsLabel(*headPtr, name)) { /* do nothing if the label already exists */
		return;
	}
	/* add the label after the check */
	addLabelNoCheck(headPtr, name, val, location, type);
}


/* gets a list and a label name and returns 1 if a label of that name exists in the list. otherwise returns 0. */
int tableContainsLabel(labelNode head, char *labelName) {
	labelNode curr = head;
	if (head == NULL) { /* list is empty */
		return 0;
	}
	do {
		if (strcmp(curr->name, labelName) == 0) { /* check for equal names */
			return 1;
		}
		curr = curr->next;
	} while (curr);

	/* then not found */
	return 0;
}

/* gets a list and a label name. if a label the that name exists in the list, it returns it's node. other wise returns 0.*/
labelNode getLabel(labelNode head, char *name) {
	labelNode curr = head;
	/* go through the list */
	while (curr) {
		if (strcmp(curr->name, name) == 0) { /* check for equal names*/
			return curr;
		}
		curr = curr->next;
	}

	/* not found */
	return NULL;
}

/* gets a list and frees its elements*/
void freetable(labelNode head) {
	labelNode fr;
	while (head) {
		fr = head;
		head = head->next;
		free(fr);
	}
}

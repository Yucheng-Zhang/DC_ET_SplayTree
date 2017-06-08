#include <stdlib.h>
#include "datatypes.h"

void insert_tree_to_dll(EulerTour *et, s_tree *n) {
	if (!et->head) { // empty list
		et->head = n;
		n->prev = n->next = NULL;
	}
	else {
		n->next = et->head;
		n->prev = NULL;
		et->head->prev = n;
		et->head = n;
	}
	et->nccomp++;
}

void delete_tree_from_dll(struct EulerTour *et, struct s_tree *n) {
	if (!n | !et)
		return;
	if (!n->prev) { // is head 
		et->head = n->next;
		if (et->head)
			et->head->prev = NULL;
	}
	else {
		n->prev->next = n->next;
		if (n->next)
			n->next->prev = n->prev;
	}
	et->nccomp--;
}

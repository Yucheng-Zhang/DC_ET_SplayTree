#pragma once
#include "datatypes.h"

/* Reset splay node @n */
static inline void reset_sn(s_node *n) {
	n->parent = n->sn_left = n->sn_right = NULL;
	n->msk &= 0b00111000; // reset root bit
	if (vertexType(n)) // a vertex node
		n->n = 1;
	else // an edge node
		n->n = 0;
}

EulerTour *init_et(u_int);

s_tree *link_et(u_int, u_int, EulerTour *);

s_tree *cut_et(u_int, u_int, EulerTour *);

byte connected_et(u_int, u_int, EulerTour *);

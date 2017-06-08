#pragma once
#include <stdlib.h>
#include "datatypes.h"

void rotate_left(struct s_node *);

void rotate_right(struct s_node *);

void splay(struct s_node *);

s_tree *split_temp(s_node *, s_tree *, s_tree *, byte);

s_tree *join(s_tree *, s_tree *, byte);

void insLast(s_tree *, s_node *);

s_tree *deleteNode(s_node *, s_tree *, byte);

byte smaller(s_node *, s_node *);

s_tree *getTree(s_node *, byte);

/* Get the first (min) node in splay tree t */
static inline s_node *s_first(s_tree *t) {
	s_node *c = t->root;
	if (!c)
		return NULL;
	while (c->sn_left) {
		c = c->sn_left;
	}
	return c;
}

/* Get the last (max) node in splay tree t */
static inline s_node *s_last(s_tree *t) {
	s_node *c = t->root;
	if (!c)
		return NULL;
	while (c->sn_right) {
		c = c->sn_right;
	}
	return c;
}

static inline s_tree *split(s_node *n, s_tree *tr, byte insLeft)
{
	return split_temp(n, tr, CAST_T malloc(sizeof(*tr)), insLeft);
}

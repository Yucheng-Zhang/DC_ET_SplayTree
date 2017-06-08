#include <stdio.h>
#include <stdlib.h>
#include "splay.h"

/* Perform a left-rotation at node @y.
 * i.e. rotate left the edge joining @y and its right child.
 */
void rotate_left(s_node *y) {

	s_node *x = RIGHT(y), *z = y->parent;
	if (!(TEST_R_BIT(y))) { // y is not root node
		if (LEFT(z) == y) // y is left child
			z->sn_left = x;
		else
			z->sn_right = x;
	}
	else { // y is the root node
		UNSET_R_BIT(y);
		(CAST_T(y->parent))->root = x;
		SET_R_BIT(x);
	}
	y->sn_right = x->sn_left;

	if (y->sn_right)
		y->sn_right->parent = y;

	x->sn_left = y;
	x->parent = z;
	y->parent = x;
	y->n = CALC_N(y);
	x->n = CALC_N(x);

}

/* Perform a right-rotation at node @y.
 * i.e. rotate right the edge joining @y and its left child.
 */
void rotate_right(s_node *y) {

	s_node *x = LEFT(y), *z = y->parent;
	if (!(TEST_R_BIT(y))) { // y is not root node
		if (LEFT(z) == y) // y is left child
			z->sn_left = x;
		else // y is right child
			z->sn_right = x;
	}
	else { // y is the root node
		UNSET_R_BIT(y);
		(CAST_T(y->parent))->root = x;
		SET_R_BIT(x);
	}
	y->sn_left = x->sn_right;

	if (y->sn_left)
		y->sn_left->parent = y;

	x->sn_right = y;
	x->parent = z;
	y->parent = x;
	y->n = CALC_N(y);
	x->n = CALC_N(x);

}

/* Perform the splay-operation starting at node @x. Note at the end of this
 * operation @x is the root of its tree and x->parent points to the head of
 * this Splay Tree
 */
void splay(s_node *x) {

	while (!(TEST_R_BIT(x))) {
		if (x == x->parent->sn_left) { // x is the left child
			if (TEST_R_BIT(x->parent)) // case 1 (zig), parent of x is tree root
				rotate_right(x->parent);
			else if (x->parent == x->parent->parent->sn_left) { // case 2 (zig-zig), x's parent is also the left child
				rotate_right(x->parent->parent);
				rotate_right(x->parent);
			}
			else { // case 3 (zig-zag), x's parent is the right child
				rotate_right(x->parent);
				rotate_left(x->parent);
			}
		}
		else if (x == x->parent->sn_right) { // x is the right child
			if (TEST_R_BIT(x->parent)) // case 1 (zig), parent of x is tree root
				rotate_left(x->parent);
			else if (x->parent == x->parent->parent->sn_right) { // case 2 (zig-zig), x's parent is also the right child
				rotate_left(x->parent->parent);
				rotate_left(x->parent);
			}
			else { // case 3 (zig-zag), x's parent is the left child
				rotate_left(x->parent);
				rotate_right(x->parent);
			}
		}
	}

}

/* Split Splay Tree given by head @tr at node @n and use @tl as head of the
 * additional new tree caused by the split.
 * @tr will hold the right part, i.e. all elements larger than n
 * @tl will hold the left part, i.e. all elements smaller than n
 * @insLeft specifies either @n will be inserted into tr (0) or tl (1)
 * return new @tl
 */
s_tree *split_temp(s_node *n, s_tree *tr, s_tree *tl, byte insLeft) {

	if (!EMPTY_TREE(tr)) {
		if (!tl)
			return NULL;
		splay(n);
		if (insLeft) { // @n will be inserted into the left part
			tr->root = n->sn_right; // set right part
			if (tr->root) {
				SET_R_BIT(tr->root);
				tr->root->parent = CAST_N tr;
			}
			if (n->sn_right)
				n->n -= n->sn_right->n; // refresh n
			n->sn_right = NULL; // set left part
			tl->root = n;
			n->parent = CAST_N tl;
		}
		else { // @n will be inserted into the right part
			tl->root = n->sn_left; // set left part
			if (tl->root) {
				tl->root->parent = CAST_N tl;
				SET_R_BIT(tl->root);
			}
			if (n->sn_left)
				n->n -= n->sn_left->n; // refresh n
			n->sn_left = NULL; // set right part
		}
		return tl;
	}

	return NULL;
}

/* Join the Splay trees given by head @tleft and @tright so that
 * all elements in @tleft's tree are followed by (<=) any element of @tright's
 * tree.
 * @useLeft indidicates which Splay tree head is used for the new joined
 * splay tree and hence returned by the function.
 * 0 results in usage of tright
 * 1 results in usage of tleft
 */
s_tree *join(s_tree *tleft, s_tree *tright, byte useLeft) {
	if (useLeft) { // tleft is used as head for the new joined splay tree
		if (EMPTY_TREE(tleft)) {
			if (!EMPTY_TREE(tright))
				tleft->root = tright->root;
			tleft->root->parent = CAST_N tleft;
		}
		else {
			s_node *mid;
			mid = s_last(tleft);
			splay(mid);
			mid->sn_right = tright->root;
			mid->n = CALC_N(mid); // refresh n
			if (tright->root) {
				tright->root->parent = mid;
				UNSET_R_BIT(tright->root); // set root
			}
		}
		return tleft;
	}
	else { // tright is used as head for the new joined splay tree
		if (EMPTY_TREE(tright)) {
			if (!EMPTY_TREE(tleft))
				tright->root = tleft->root;
			tright->root->parent = CAST_N tright;
		}
		else {
			s_node *mid;
			mid = s_first(tright);
			splay(mid);
			mid->sn_left = tleft->root;
			mid->n = CALC_N(mid); // refresh n
			if (tleft->root) {
				tleft->root->parent = mid;
				UNSET_R_BIT(tleft->root); // set root
			}
		}
		return tright;
	}
}

/* Insert the node @n into the Splay tree given by head t as the last (max)
 * element and afterwards (important for the amortized runtime) splay this
 * node @n
 */
void insLast(s_tree *t, s_node *n) {
	s_node *c = s_last(t);
	if (!c) {
		t->root = n;
		n->parent = CAST_N t;
		SET_R_BIT(n);
		n->n = CALC_N(n);
	}
	else {
		c->sn_right = n;
		n->parent = c;
		c->n = CALC_N(c);
	}
	splay(n);
}

/* Remove node @d from tree given by head @t. If del is 1 the node will also
 * be freed. The head of the resulting tree is returned.
 */
s_tree *deleteNode(s_node *d, s_tree *t, byte del) {
	s_tree *tmp_tree;
	tmp_tree->root = NULL;
	tmp_tree->next = tmp_tree->prev = NULL;
	if (EMPTY_TREE(t))
		return t;
	splay(d);
	t->root = d->sn_left;
	if (d->sn_left) {
		d->sn_left->parent = CAST_N t;
		SET_R_BIT(t->root);
	}
	tmp_tree->root = d->sn_right;
	if (d->sn_right) {
		d->sn_right->parent = CAST_N &tmp_tree;
		SET_R_BIT(tmp_tree->root);
	}
	t = join(t, tmp_tree, 1);
	if (del)
		free(d);

	return t;
}

/* Returns 0 if @a >=@b and 1 if @a<@b */
byte smaller(s_node *a, s_node *b) {
	splay(a);
	splay(b);
	if (a == b->sn_left)
		return 1;
	if (a == b->sn_right)
		return 0;
	if (a == (b->sn_left ? b->sn_left->sn_left : NULL))
		return 1;
	if (a == (b->sn_right ? b->sn_right->sn_right : NULL))
		return 0;
	return 2;
}

/* Return the head of tree in which node @c resides. If withSplay is 1 then
 * @c is splayed to the root also.
 */
s_tree *getTree(s_node *c, byte withSplay) {
	if (!c)
		return NULL;
	if (withSplay) {
		splay(c);
		return CAST_T c->parent;
	}
	else {
		s_node *n;
		n = c;
		while (n) {
			if (TEST_R_BIT(n)) { // n comes to the root node
				return CAST_T n->parent;
			}
			else
				n = n->parent;
		}
	}
	return NULL;
}

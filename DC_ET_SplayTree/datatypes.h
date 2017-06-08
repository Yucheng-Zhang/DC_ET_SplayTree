#pragma once

typedef unsigned char byte;
typedef unsigned int u_int;

/* Bit for vertex : 0b00100000 */

#define RMASK 0b00000100 // root bit

/* Set the corresponding bit to 1 */
#define SET_R_BIT(a) (a->msk |= RMASK)

/* Set the corresponding bit to 0 */
#define UNSET_R_BIT(a) (a->msk = (a->msk)&(~(RMASK)))

#define TEST_R_BIT(a) (a->msk)&RMASK

#define N(node) (node ? (node->n) : 0)
#define CALC_N(node) ( (vertexType(node)? 1: 0 ) + N(node->sn_left) + N(node->sn_right))

#define CAST_N (s_node *)
#define CAST_T (s_tree *)

#define SET_UP(a)    ((a.msk) |= 0b00000000)
#define SET_RIGHT(a) ((a.msk) |= 0b00001000)
#define SET_DOWN(a)  ((a.msk) |= 0b00010000)
#define SET_LEFT(a)  ((a.msk) |= 0b00011000)

#define PARENT(node) (node ? node->parent : NULL )
#define LEFT(node) (node ? node->sn_left : NULL)
#define RIGHT(node) (node ? node->sn_right : NULL)
#define GPARENT(node) (PARENT(PARENT(node)))
#define EMPTY_TREE(tree) (!tree->root)
#define INIT_NODE(n,k,p1, p2) do {n->parent = n->sn_left = n->sn_right = NULL; n->key = k; n->msk = 0; setDat(n,p1,p2); n->n = 1;}while(0)

typedef struct Edge {
	u_int from, to;
	byte type; // active or inactive edge

} Edge;

/* The structure of a node in splay tree */
typedef struct s_node {
	u_int key; // The No. of the vertices

	s_node *parent;
	s_node *sn_left;
	s_node *sn_right;
	/* Used to store the properties of the node:
	 * root, vertex or edge(with direction)
	 * in different bits
	 */
	byte msk;
	u_int n; // Number of vertex nodes in the subtree (including self)

} s_node;

/* The structure of a splay tree */
typedef struct s_tree {
	s_node *root;
	s_tree *next; // Pointer for double linked list of trees
	s_tree *prev; // Pointer for double linked list of trees
} s_tree;

/* The structure of the Euler Tour over the whole lattice */
typedef struct EulerTour {
	u_int nedges; // Number of (active) edges in this spanning forest/ET
	s_node *edges; // Nodes of all edges(with direction in Euler Tour) in the splay trees
	s_node *vertices; // Nodes of all vertices(in Euler Tour) in the splay trees

	/* The splay trees of the Euler Tour Sequence are linked in a double linked list */
	s_tree *head; // Head of double linked list of trees
	s_tree *tail; // Tail of double linked list of trees
	u_int nccomp; // Number of trees in double linked list
} EulerTour;

/* Set the type of the node to vertex */
static inline byte vertexType(const s_node *c) {
	if (!c)
		return 0;
	else
		return c->msk & 0b001000000; // vertex
}

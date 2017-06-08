#include <stdlib.h>
#include "eulertour.h"
#include "datatypes.h"
#include "dllist.h"
#include "splay.h"

static u_int DX;

/* Return the node corresponding to the Euler Tour linearization of edge
 * (@u,@v)
 */
static inline s_node *getE_SN(const u_int u, const u_int v, s_node *edges)
{
	// Note: 0:up, 1:right, 2:down, 3:left
	if ((u - v == DX) || (v - u == DX*(DX - 1))) { // u to v up
		return &edges[u * 4];
	}
	if ((v - u == DX) || (u - v == DX*(DX - 1))) { // u to v down
		return &edges[u * 4 + 2];
	}
	if ((v - u + 1 == DX) || (u - v == 1)) { // u to v right
		return &edges[u * 4 + 3];
	}
	if ((u - v + 1 == DX) || (v - u == 1)) { // u to v left
		return &edges[u * 4 + 1];
	}
	return NULL; // means not a valid edge
}

/* Initialize EulerTour datastructure for a two dimensional square lattice
 * graph with periodic boundary conditions and one dimensional length @dlength
 * All nodes of vertices and edges are created for future usage, but not linked.
 */
EulerTour *init_et(u_int dlength)
{
	u_int nvertices = dlength*dlength;
	EulerTour *et = (EulerTour *)malloc(sizeof(EulerTour));
	et->head = et->tail = NULL;
	et->nccomp = 0;
	et->nedges = 0;
	et->vertices = (s_node *)malloc(nvertices * sizeof(s_node));
	et->edges = (s_node *)malloc(nvertices * 4 * sizeof(s_node));
	/* Initialize vertex nodes */
	for (u_int i = 0; i < nvertices; i++) {
		et->vertices[i].key = i;
		et->vertices[i].msk = 0b00100000; // Mask bit for vertex
		reset_sn(&et->vertices[i]);

		et->vertices[i].parent = CAST_N malloc(sizeof(s_tree));
		SET_R_BIT((&et->vertices[i])); // Set root bit
		(CAST_T(et->vertices[i].parent))->root = &et->vertices[i];
		insert_tree_to_dll(et, CAST_T(et->vertices[i].parent));
	}
	/* Initialize edge nodes */
	for (u_int i = 0; i < nvertices; i++) {
		et->edges[4 * i].key = i;
		et->edges[4 * i].msk = 0;
		reset_sn(&et->edges[4 * i]);
		SET_UP(et->edges[4 * i]); // point up

		et->edges[4 * i + 1].key = i;
		et->edges[4 * i + 1].msk = 0;
		reset_sn(&et->edges[4 * i + 1]);
		SET_RIGHT(et->edges[4 * i + 1]); // point right

		et->edges[4 * i + 2].key = i;
		et->edges[4 * i + 2].msk = 0;
		reset_sn(&et->edges[4 * i + 2]);
		SET_DOWN(et->edges[4 * i + 2]); // point down

		et->edges[4 * i + 3].key = i;
		et->edges[4 * i + 3].msk = 0;
		reset_sn(&et->edges[4 * i + 3]);
		SET_LEFT(et->edges[4 * i + 3]); // point left
	}
	return et;
}

/* Link the Euler Tours of spanning trees of vertices @u and @v.
 * Precondition: @u, @v are not connected in Spanning Forest @et
 */
s_tree *link_et(u_int u, u_int v, EulerTour *et)
{
	s_node *un = &et->vertices[u], *vn = &et->vertices[v]; // the nodes of vertex u and vertex v
	s_tree *ut = getTree(un, 1), *vt = getTree(vn, 1), *ut1, *vt1;
	s_node *euv = getE_SN(u, v, et->edges), *evu = getE_SN(v, u, et->edges); // nodes of edge (u,v) and (v,u)
	if (ut == vt)
		return ut;
	// Merge the two euler tours
	s_tree *tmp_tree = (s_tree *)malloc(sizeof(s_tree));
	tmp_tree->root = NULL;
	tmp_tree->next = tmp_tree->prev = NULL;
	ut1 = split_temp(un, ut, tmp_tree, 1);
	ut = join(ut, ut1, 1);
	insLast(ut, euv);
	vt1 = split_temp(vn, vt, tmp_tree, 1);
	vt = join(vt, vt1, 1);
	ut = join(ut, vt, 1);
	delete_tree_from_dll(et, vt); // delete tree from list of trees
	free(vt);
	// Add a last occurrence
	insLast(ut, evu);
	return ut;
}

/* Cut spanning tree caused by deletion of edge (@u,@v) into two
 * separate spanning trees. Return a pointer to the new one.
 */
s_tree *cut_et(u_int u, u_int v, EulerTour *et)
{
	s_node *e1 = getE_SN(u, v, et->edges); // u -> v in Euler Tour
	s_node *e2 = getE_SN(v, u, et->edges); // v -> u in Euler Tour
	s_node *tmp_n;
	s_tree *t = getTree(e1, 1), *t1, *t2 = getTree(e2, 1);

	/* smaller with respect to the order in the Euler Tour */
	if (!smaller(e1, e2)) {
		tmp_n = e1;
		e1 = e2;
		e2 = tmp_n;
	} // make e1 < e2
	s_tree *tmp_tree = (s_tree *)malloc(sizeof(s_tree));
	tmp_tree->root = NULL;
	tmp_tree->next = tmp_tree->prev = NULL;
	t1 = split_temp(e1, t, tmp_tree, 0); // split before e1
	t2 = split(e2, t, 1); // split after e2
	t = join(t1, t, 0);

	t2 = deleteNode(e1, t2, 0);
	reset_sn(e1);
	t2 = deleteNode(e2, t2, 0);
	reset_sn(e2);
	insert_tree_to_dll(et, t2);
	return t2;
}

/* Check if Vertices @u and @v are connected in EulerTour @et */
byte connected_et(u_int u, u_int v, EulerTour *et)
{
	return getTree(&et->vertices[u], 0) == getTree(&et->vertices[v], 0);
}

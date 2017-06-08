#include <stdlib.h>
#include "datatypes.h"
#include "dyncon.h"
#include "eulertour.h"

/* Initialize dynamic connectivity data structures for a planar 2d square
 * lattice graph with periodic boundary conditions and one dimensional size @dl
 */
void init_dc(u_int dl)
{
	dlength = dl;
	nvertices = dlength*dlength; // 2d square lattice
	te = 0;
	ET = init_et(dlength); // Creat & Initialize Euler Tour of the lattice
}

/* Insert Edge @e into the dynamic connectivity data structure.
 * Precondition: @e is not already part of the graph.
 * Precondition: @e is tree edge, i.e. @e will link two different trees.
 */
void insert_dc(Edge *e)
{
	te++;
	link_et(e->from, e->to, ET);
}

/* Delete Edge @e from the dynamic connectivity datastructure.
 * Precondition: @e is part of the graph.
 */
void delete_dc(Edge *e)
{
	te--;
	cut_et(e->from, e->to, ET);
}

/* Check whether vertices @u and @v are connected.
 */
byte connected_dc(u_int u, u_int v)
{
	return connected_et(u, v, ET);
}

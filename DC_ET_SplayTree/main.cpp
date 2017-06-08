#include <stdio.h>
#include <stdlib.h>
#include "dyncon.h"

EulerTour *ET; // The Euler Tour over the whole lattice
Edge *adjMatrix; // Store the info of edges directly

u_int dlength; // Linear size of the square lattice
u_int nvertices; // Number of vertices

u_int te; // Number of tree edges

void init_adjMatrix()
{
	/* Initialize adjacency Matrix */
	adjMatrix = (Edge *)malloc(2 * nvertices * sizeof(Edge));
	for (u_int i = 0; i < nvertices; i++) {
		adjMatrix[2 * i].type = 0; // inactive edge  
		adjMatrix[2 * i].from = i;
		adjMatrix[2 * i].to = (i < dlength ? i + dlength*(dlength - 1) : i - dlength); // up edge

		adjMatrix[2 * i + 1].type = 0;
		adjMatrix[2 * i + 1].from = i;
		adjMatrix[2 * i + 1].to = ((i + 1) % (dlength) ? i + 1 : i - dlength + 1); // right edge
	}
}

int main()
{
	printf("Dynamic Connectivity with Euler Tour in Splay Tree.\n");
	init_dc(4);

	init_adjMatrix();

	adjMatrix[3].type = 1;
	insert_dc(&adjMatrix[3]);

	printf("%d\n", connected_dc(1, 2));
	return 0;
}

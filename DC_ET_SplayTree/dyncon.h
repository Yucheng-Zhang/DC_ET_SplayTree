#pragma once
#include "datatypes.h"

extern EulerTour *ET;

extern u_int dlength; // Linear size of the square lattice
extern u_int nvertices; // Number of vertices

extern u_int te; // Number of tree edges

void init_dc(u_int);

void insert_dc(Edge *);

void delete_dc(Edge *);

byte connected_dc(u_int, u_int);

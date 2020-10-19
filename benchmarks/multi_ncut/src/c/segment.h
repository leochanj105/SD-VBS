/********************************
Author: Sravanthi Kota Venkata
********************************/

#ifndef SEGMENT
#define SEGMENT

#include "sdvbs_common.h"

typedef struct
{
  float w;
  int a, b;
} edge;

typedef struct 
{
  int rank;
  int p;
  int size;
} uni_elt;

typedef struct
{
    uni_elt *elts;
    int num;
}universe;

/* use imRef to access image data. */
#define imRef(im, x, y) (im->data[y*im->width+x])

I2D *segment_image(I2D *im, float sigma, float c, int min_size, int *num_ccs, 
		   int* segments, edge* edges,
		   F2D* imageOut, F2D* tempOut, I2D* kernel, F2D* edgeWeights, F2D* in, I2D* ind,
		   universe* u,
		   I2D* output);
universe *segment_graph(int num_vertices, int num_edges, edge *edges, float c, F2D* edgeWeights, F2D* in, I2D* ind,
		        universe* u);
void join(universe* u, int x, int y);
int find(universe* u, int x);
float diff(F2D *r, int x1, int y1, int x2, int y2);


#endif



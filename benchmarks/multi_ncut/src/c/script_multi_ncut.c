/********************************
Author: Sravanthi Kota Venkata
********************************/

#include <stdio.h>
#include <stdlib.h>
#include "segment.h"

#define MAX_EDGE 500000
int main(int argc, char* argv[])
{
    float sigma = 0.6;
    float k = 4;
    int min_size = 10;
    char im1[256];
    int num_ccs[1] = {0};
    I2D *out;
    I2D* im;
    I2D* seg;
    I2D* kernel;
    I2D* ind;
    I2D* output;
    unsigned int* start, *endC, *elapsed;
    int ret;
    int *segments;
    edge* edges;
    universe* u;

    F2D *imageOut, *tempOut;
    F2D *edgeWeights, *in;
    if(argc < 2)
    {
        printf("We need input image path and output path\n");
        return -1;
    }

    sprintf(im1, "%s/1.bmp", argv[1]);
    im = readImage(im1);


    int height, width, num_edges, num_vertices;
    height = im->height;
    width = im->width;
    num_edges = MAX_EDGE;
    num_vertices = width * height;
    segments = (int *) malloc(height*width*sizeof(int));
    edges = (edge*)malloc(sizeof(edge)*width*height*4);
    imageOut = fSetArray(height, width, 0);
    tempOut = fSetArray(height, width, 0);
    kernel = iMallocHandle(1,5);
    edgeWeights = fMallocHandle(1, num_edges);
    in = fDeepCopy(edgeWeights);
    ind = fMallocHandle(1, num_edges);
    u = (universe*)malloc(sizeof(universe));
    u->elts = (uni_elt*)malloc(sizeof(uni_elt)*num_vertices);
    output = iMallocHandle(height, width);

    printf("Input size\t\t- (%dx%d)\n", im->height, im->width);
    sbrk(1<<4);
int iter = 5;
for(int i = 0; i < iter; i++){
	printf("iteration %d\n", i);
    start = photonStartTiming();
    seg = segment_image(im, sigma, k, min_size, num_ccs, segments, edges, imageOut, tempOut, kernel, edgeWeights, in, ind, u, output);
    endC = photonEndTiming();
    elapsed = photonReportTiming(start, endC);
    out = seg;

#ifdef CHECK   
    /** Self checking - use expected.txt from data directory  **/
    {
        int ret=0;
        float tol = 0;
        
#ifdef GENERATE_OUTPUT
        writeMatrix(out, argv[1]);
#endif
 
    ret = selfCheck(out, argv[1], tol);
    if(ret < 0)
        printf("Error in Multi N Cut\n");
    }
#endif

    photonPrintTiming(elapsed);
}
sbrk(1<<4);
    iFreeHandle(im); 
    free(edges);
    free(segments);
    free(tempOut);
    free(imageOut);
    free(edgeWeights);
    free(in);
    free(ind);
    free(output);
    free(u->elts);
    free(u);
    return 0;
}



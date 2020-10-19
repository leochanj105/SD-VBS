/********************************
Author: Sravanthi Kota Venkata
********************************/

#include "mser.h"
#include <malloc.h>
#define min(a,b) (a<b)?a:b
#define max(a,b) (a>b)?a:b

int main(int argc, char* argv[])
{

    int which_image;
    int i, j, k;
    I2D *idx;
    I2D *I;
    I2D *It;
    I2D *out;
    int rows=196, cols=98;
    int minVal = 1000;
    int maxVal = -1000;
    int lev = 10;

    unsigned int* start, *endC, *elapsed;
    char im1[100], im2[100];

    iArray *subs_pt, *nsubs_pt, *strides_pt, *visited_pt, *dims;   
    uiArray* joins_pt;
    ulliArray *acc_pt, *ell_pt;
    region_t* regions_pt;
    pair_t* pairs_pt;
    node_t* forest_pt;

    int ndims, nel, gdl, nmer;

    if(argc < 2)
    {
        printf("We need input image path\n");
        return -1;
    }

    sprintf(im1, "%s/1.bmp", argv[1]);



    I = readImage(im1);

    rows = I->height;
    cols = I->width;

    It = readImage(im1);

    k = 0;
    for(i=0; i<cols; i++)
    {
        for(j=0; j<rows; j++)
        {
            asubsref(It,k++) = subsref(I,j,i);
        }
    }
    
    ndims = 2;
    nel = It->height * It->width;
    gdl = ndims * (ndims+1)/2 + ndims;
    nmer = NMER_MAX;

    dims = malloc(sizeof(iArray) + sizeof(int)*ndims);
    /* allocate stuff */
    subs_pt = malloc(sizeof(iArray) + sizeof(int)*ndims);
    nsubs_pt = malloc(sizeof(iArray) + sizeof(int)*ndims);
    strides_pt = malloc(sizeof(uiArray)+sizeof(unsigned int)*ndims);
    visited_pt = malloc(sizeof(uiArray) + sizeof(unsigned int)*nel);
    joins_pt = malloc(sizeof(uiArray) + sizeof(unsigned int)*nel);

    regions_pt = (region_t*)malloc(sizeof(region_t)*nel);
    pairs_pt = (pair_t*)malloc(sizeof(pair_t)*nel);
    forest_pt = (node_t*)malloc(sizeof(node_t)*nel);

    acc_pt = malloc(sizeof(ulliArray) + sizeof(acc_t)*nel) ;
    ell_pt = malloc(sizeof(ulliArray) + sizeof(acc_t)*gdl*nmer) ; 


    out = iMallocHandle(1, nmer);
    //start
    int iter = 5;
    for(int it = 0; it < iter; it++){
	    printf("Iteration %d\n", it);
    start = photonStartTiming();
    idx = mser(It, 2, subs_pt, nsubs_pt, strides_pt, visited_pt, dims, 
	       joins_pt,
	       regions_pt,
	       pairs_pt,
	       forest_pt,
	       acc_pt, ell_pt,
	       out);
    endC = photonEndTiming();
    elapsed = photonReportTiming(start, endC);
    //free(tt);
    printf("Input size\t\t- (%dx%d)\n", rows, cols);
#ifdef CHECK   
    /** Self checking - use expected.txt from data directory  **/
    {
        int tol, ret=0;
        tol = 1;
#ifdef GENERATE_OUTPUT
        writeMatrix(idx, argv[1]);
#endif
        ret = selfCheck(idx, argv[1], tol);
        if (ret == -1)
            printf("Error in MSER\n");
    }
    /** Self checking done **/
#endif    
    photonPrintTiming(elapsed);
    }
   printf("end..\n");
  //end 
    //free(start); 
    //free(endC); 
    //free(elapsed); 
    
    free(dims);
    free( forest_pt  ) ;
    free( pairs_pt   ) ;
    free( regions_pt ) ;
    free( visited_pt ) ;
    free( strides_pt ) ;
    free( nsubs_pt   ) ;
    free( subs_pt    ) ;
    free( joins_pt    ) ;
    free( acc_pt ) ;
    free( ell_pt ) ;
    iFreeHandle(idx);
    iFreeHandle(I);
    iFreeHandle(It);
    return 0;
}


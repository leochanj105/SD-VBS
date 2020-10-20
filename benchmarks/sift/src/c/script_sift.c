/********************************
Author: Sravanthi Kota Venkata
********************************/

#include <stdio.h>
#include <stdlib.h>
#include "sift.h"
#include <malloc.h>
#define SIFT_MEM 1<<29
void normalizeImage(F2D* image)
{
	int i;
	int rows;
	int cols;
 
    int tempMin = 10000, tempMax = -1;
	rows = image->height;
	cols = image->width;

    for(i=0; i<(rows*cols); i++)
        if(tempMin > asubsref(image,i))
            tempMin = asubsref(image,i);

    for(i=0; i<(rows*cols); i++)
        asubsref(image,i) = asubsref(image,i) - tempMin;

    for(i=0; i<(rows*cols); i++)
        if(tempMax < asubsref(image,i))
            tempMax = asubsref(image,i);

    for(i=0; i<(rows*cols); i++)
        asubsref(image,i) = ( asubsref(image,i) / (tempMax+0.0) );
}

int main(int argc, char* argv[])
{
    I2D* im;
    F2D *image;
    int rows, cols, i, j;
    F2D* frames;
    unsigned int* startTime, *endTime, *elapsed;    
    
    char imSrc[100];
    
    if(argc < 2)
    {
        printf("We need input image path\n");
        return -1;
    }

    mallopt(M_TOP_PAD, SIFT_MEM);
    mallopt(M_MMAP_MAX, 0);
    sprintf(imSrc, "%s/1.bmp", argv[1]);

    im = readImage(imSrc);
    image = fiDeepCopy(im);
    //iFreeHandle(im);
    rows = image->height;
    cols = image->width;


    int iter = 20;
    printf("start\n");

   for(int it = 0; it < iter; it++){
	    printf("Iteration %d\n", it);
    startTime = photonStartTiming();
    /** Normalize the input image to lie between 0-1 **/

    image = fiDeepCopy(im);
	normalizeImage(image);

    /** Extract sift features for the normalized image **/
    frames = sift(image);

    endTime = photonEndTiming();

    printf("Input size\t\t- (%dx%d)\n", rows, cols);
   
#ifdef CHECK   
    {
        int ret=0;
        float tol = 0.2;
#ifdef GENERATE_OUTPUT
        fWriteMatrix(frames, argv[1]);
#endif
        ret = fSelfCheck(frames, argv[1], tol);
        if (ret == -1)
            printf("Error in SIFT\n");
    }
#endif    

    elapsed = photonReportTiming(startTime, endTime);
    photonPrintTiming(elapsed);
    }
    printf("end\n");
    free(startTime);
    free(endTime);
    free(elapsed);
    iFreeHandle(im);
    //fFreeHandle(image);
    fFreeHandle(frames);

    return 0;
}


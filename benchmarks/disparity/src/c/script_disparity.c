/********************************
Author: Sravanthi Kota Venkata
********************************/

#include <stdio.h>
#include <stdlib.h>
#include "disparity.h"
#include <malloc.h>

int main(int argc, char* argv[])
{
    int rows = 32;
    int cols = 32;
    I2D *imleft, *imright, *retDisparity;
    I2D *IrightPadded, *IleftPadded, *Iright_moved;
    I2D *retDisp, *halfWin;
    I2D *range;
    F2D *retSAD, *minSAD, *SAD, *integralImg;
    unsigned int *start, *endC, *elapsed;
    
    int i, j;
    char im1[100], im2[100], timFile[100];
    int WIN_SZ=8, SHIFT=64;
    FILE* fp;
    
    if(argc < 2)
    {
        printf("We need input image path and output path\n");
        return -1;
    }

    sprintf(im1, "%s/1.bmp", argv[1]);
    sprintf(im2, "%s/2.bmp", argv[1]);
    
    imleft = readImage(im1);
    imright = readImage(im2);

    rows = imleft->height;
    cols = imleft->width;

#ifdef test
    WIN_SZ = 2;
    SHIFT = 1;
#endif
#ifdef sim_fast
    WIN_SZ = 4;
    SHIFT = 4;
#endif
#ifdef sim
    WIN_SZ = 4;
    SHIFT = 8;
#endif


    int nr, nc, k;
    int half_win_sz;
    nr = imleft->height;
    nc = imleft->width;
    half_win_sz = WIN_SZ / 2;

    minSAD = fSetArray(nr, nc, 255.0*255.0);
    retDisp = iSetArray(nr, nc, SHIFT);
    halfWin = iSetArray(1,2,half_win_sz);


    if(WIN_SZ > 1)
    {
        IleftPadded = padarray2(imleft, halfWin);
        IrightPadded = padarray2(imright, halfWin);
    }
    else
    {
        IleftPadded = imleft;
        IrightPadded = imright;
    }

    int paddedRows, paddedCols;
    paddedRows = IleftPadded->height;
    paddedCols = IleftPadded->width;
    SAD = fSetArray(paddedRows, paddedCols,255);
    integralImg = fSetArray(paddedRows, paddedCols,0);
    retSAD = fMallocHandle(paddedRows- WIN_SZ, paddedCols - WIN_SZ);
    Iright_moved = iSetArray(paddedRows, paddedCols, 0);

    range = iMallocHandle(1, 2);

    //int *sss = (int*)malloc(sizeof(int) * 32768000);
    //mallopt(M_TOP_PAD, 1 << 24);
    //mallopt(M_MMAP_MAX, 0);
    //starting point of benchmarks
    printf("starting..\n");
    int iter = 5;
    //sbrk(1<<4);
    for(int i = 0; i < iter; i++){
	    printf("Iteration %d:\n", i);

	    //int *ppp = (int*)malloc(sizeof(int) * 32768);

    start = photonStartTiming();
    retDisparity = getDisparity(imleft, imright, WIN_SZ, SHIFT, 
		   minSAD, retDisp, halfWin,
		   IrightPadded, IleftPadded, Iright_moved,
		   retSAD, SAD, integralImg,
		   range);
    endC = photonEndTiming();
    elapsed = photonReportTiming(start, endC);
    photonPrintTiming(elapsed);
    printf("Input size\t\t- (%dx%d)\n", rows, cols);

    //free(ppp);

#ifdef CHECK   
    /** Self checking - use expected.txt from data directory  **/
    {
        int tol, ret=0;
        tol = 2;
#ifdef GENERATE_OUTPUT
        writeMatrix(retDisparity, argv[1]);
#endif
        ret = selfCheck(retDisparity, argv[1], tol);
        if (ret == -1)
            printf("Error in Disparity Map\n");
    }
    /** Self checking done **/
#endif

    }
    printf("ending\n");
    //end of benchmark
    //sbrk(1<<4);
    //fFreeHandle(retSAD);
    //free(sss);
    fFreeHandle(minSAD);
    fFreeHandle(SAD);
    fFreeHandle(integralImg);
    iFreeHandle(IrightPadded);
    iFreeHandle(IleftPadded);
    iFreeHandle(Iright_moved);
    fFreeHandle(retSAD);
    iFreeHandle(imleft);
    iFreeHandle(imright);
    iFreeHandle(retDisparity);
    iFreeHandle(halfWin);
    iFreeHandle(range);
    free(start);
    free(endC);
    free(elapsed);

    return 0;
}

/********************************
Author: Sravanthi Kota Venkata
********************************/

#include <stdio.h>
#include <stdlib.h>
#include "disparity.h"

I2D* getDisparity(I2D* Ileft, I2D* Iright, int win_sz, int max_shift,
		  I2D* minSAD, I2D* retDisp, I2D* halfWin, 
		  I2D* IrightPadded, I2D* IleftPadded, I2D* Iright_moved,
		  F2D* retSAD, F2D* SAD, F2D* integralImg,
		  I2D* range)
{
    int nr, nc, k;
    int half_win_sz, rows, cols;
    
    nr = Ileft->height;
    nc = Ileft->width;
    half_win_sz=win_sz/2;
    
    
    fResetArray(minSAD, nr, nc, 255.0*255.0);
    iResetArray(retDisp, nr, nc,max_shift);
    iResetArray(halfWin, 1,2,half_win_sz);

    rows = IleftPadded->height;
    cols = IleftPadded->width;
    fResetArray(SAD, rows, cols,255);
    fResetArray(integralImg, rows, cols,0);
    //fResetArray(retSAD,rows-win_sz, cols-win_sz, 0);
    iResetArray(Iright_moved, rows, cols, 0);
    
    for( k=0; k<max_shift; k++)
    {    
        correlateSAD_2D(IleftPadded, IrightPadded, Iright_moved, win_sz, k, SAD, integralImg, retSAD, range);
        findDisparity(retSAD, minSAD, retDisp, k, nr, nc);
    }
    
     
    return retDisp;
}


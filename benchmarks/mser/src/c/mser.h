/********************************
Author: Sravanthi Kota Venkata
********************************/

#ifndef _MSER_
#define _MSER_

#define sref(a,i) a->data[i]

#include "sdvbs_common.h"
#define NMER_MAX 756

typedef int val_t;

typedef struct
{
    int width;
    int data[];
}iArray;

typedef struct
{
    int width;
    unsigned int data[];
}uiArray;

typedef struct
{
    int width;
    long long int unsigned data[];
}ulliArray;

#define MIN(a,b) (a<b)?a:b
#define MAX(a,b) (a>b)?a:b

typedef int  unsigned idx_t ;
typedef long long int unsigned acc_t ;

/* pairs are used to sort the pixels */
typedef struct 
{
  val_t value ; 
  idx_t  index ;
} pair_t ;

/* forest node */
typedef struct
{
  idx_t parent ;   /**< parent pixel         */
  idx_t shortcut ; /**< shortcut to the root */
  idx_t region ;   /**< index of the region  */
  int   area ;     /**< area of the region   */
#ifdef USE_RANK_UNION
  int   height ;    /**< node height         */
#endif
} node_t ;

/* extremal regions */
typedef struct
{
  idx_t parent ;     /**< parent region                           */
  idx_t index ;      /**< index of root pixel                     */
  val_t value ;      /**< value of root pixel                     */
  int   area ;       /**< area of the region                      */
  int   area_top ;   /**< area of the region DELTA levels above   */
  int   area_bot  ;  /**< area of the region DELTA levels below   */
  float variation ;  /**< variation                               */
  int   maxstable ;  /**< max stable number (=0 if not maxstable) */
} region_t ;

int script_mser();
I2D* mser(I2D* I, int in_delta,  
          iArray* subs_pt, iArray* nsubs_pt, iArray* strides_pt, iArray* visited_pt, iArray* dims, 
	  uiArray* joins_pt,
	  region_t* regions_pt, 
	  pair_t* pairs_pt,
	  node_t* forest_pt,
	  ulliArray* acc_pt, ulliArray* ell_pt,
	  I2D* out);

#endif



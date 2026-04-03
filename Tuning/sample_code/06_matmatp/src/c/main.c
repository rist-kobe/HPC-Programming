/* Copyright 2024 Research Organization for Information Science and Technology */
#include <stdlib.h>
#include <stdio.h>
#include "mykernel.h"
#include "timer.h"

/* Please set NSIZE for double precision arrays satisfying the following conditions.  
 * (1)   8bytes*(NSIZE)^2 > (L2 cache size per core)
 * (2) 3*8bytes*(NSIZE)^2 ~ (Target memory size)
 * A typical choice of (Target memory size) is to set the size of 
 * the memory whose latency is the highest in the memory level.
 *  ex1: Main memory size (per NUMA node)
 *  ex2: Size (~1.5 times) larger than LLC (per NUMA node or CMG or DIE) */
#define NSIZE 1500

/* Please set blocking size (for double precision array) of matrix transpose.
 * You can estimate it in the unit of cache line size.
 * We note that BLKSIZE_TRANS must be less than NSIZE.
 * This parameter has an impact only on mmp_lex_tp_blk2                 */
#define BLKSIZE_TRANS 40

/* Please set the number of iterations in each kernel                  */
#define NITR 30

int main (int argc, char **argv)
{
  int nbk1, nbk2, nbkr, nbkc, icon;

  double zero = 0.0;
  double one  = 1.0;
  double tmp, elp0, elp, memsize;
  double mata[NSIZE][NSIZE], matb[NSIZE][NSIZE], matc[NSIZE][NSIZE];

  if ( argc != 3 ) {
     printf("[usage] run.x (arg1) (arg2)\n");
     printf("        (arg1): blocking size for the innermost loop\n");
     printf("        (arg2): blocking size for the 2nd innermost loop\n");
     return EXIT_SUCCESS;
  }

  memsize = (8.0 * NSIZE * NSIZE) / 1048576.0;
 
  printf("--------------------\n");
  printf("Matrix dimension (double precision; 8bytes): %d\n"
         "Total memory size in kernels (MiB): %9.3f\n", NSIZE, 3.0*memsize);

  nbk1 = atoi(argv[1]);
  nbk2 = atoi(argv[2]);

  {
     tmp = 1.0 / NSIZE;
     unsigned int seed = 409101;
     srand( seed );
     for ( int i = 0; i < NSIZE; ++i ) {
     for ( int j = 0; j < NSIZE; ++j ) {
         mata[i][j] = (2.0*(double)rand()/RAND_MAX - one) * tmp;
         matb[i][j] = (2.0*(double)rand()/RAND_MAX - one) * tmp;
     }}
  }

  nbkr = BLKSIZE_TRANS;
  nbkc = BLKSIZE_TRANS;

  /*====================================================================*/
  printf("[Mat-Mat product: Simple implementation]\n");

  /* warm up */
  icon = 0;
  for ( int i = 0; i < 3; ++i ) {
    mmp_simple(NSIZE, matc, mata, matb);
    icon += dummy(NSIZE, matc);
  }

  elp0 = get_elp_time();

  icon = 0;
  for ( int i = 0; i < NITR; ++i ) {
    mmp_simple(NSIZE, matc, mata, matb);
    icon += dummy(NSIZE, matc);
  }

  elp  = get_elp_time() - elp0;

  {
     tmp = zero;
     for ( int i = 0; i < NSIZE; ++i) {
         tmp += matc[i][i];
     }
  }
  printf("Elapsed (sec) %10.4f,# of nitr. = %d, trace =  %23.12e\n",elp, NITR, tmp);
  /*====================================================================*/
  printf("[Mat-Mat product: Simple implementation with loop blocking]\n"); 

  memsize = (8.0*(nbk1*nbk2 + nbk1 + nbk2))/1024.0;

  printf("Estimated data size per block (KiB): %9.3f\n", memsize);

  /* warm up */
  icon = 0;
  for ( int i = 0; i < 3; ++i ) {
    mmp_simple_blk(NSIZE, nbk1, nbk2, matc, mata, matb);
    icon += dummy(NSIZE, matc);
  }

  elp0 = get_elp_time();

  icon = 0;
  for ( int i = 0; i < NITR; ++i ) {
    mmp_simple_blk(NSIZE, nbk1, nbk2, matc, mata, matb);
    icon += dummy(NSIZE, matc);
  }

  elp  = get_elp_time() - elp0;

  {
     tmp = zero;
     for ( int i = 0; i < NSIZE; ++i) {
         tmp += matc[i][i];
     }
  }
  printf("Elapsed (sec) %10.4f,# of nitr. = %d, trace =  %23.12e\n",elp, NITR, tmp);
  /*====================================================================*/
  printf("[Mat-Mat product: Loop exchanging, Transpose, " 
         "Blocking on two inner loops]\n");

  memsize = (8.0*(nbk1*nbk2 + nbk1))/1024.0;

  printf("Estimated data size per block (KiB): %9.3f\n", memsize);

  /* warm up */
  icon = 0;
  for ( int i = 0; i < 3; ++i ) {
     mmp_lex_tp_blk(NSIZE, nbk1, nbk2, nbkr, nbkc, matc, mata, matb);
     icon += dummy(NSIZE, matc);
  }

  elp0 = get_elp_time();

  icon = 0;
  for ( int i = 0; i < NITR; ++i ) {
     mmp_lex_tp_blk(NSIZE, nbk1, nbk2, nbkr, nbkc, matc, mata, matb);
     icon += dummy(NSIZE, matc);
  }

  elp  = get_elp_time() - elp0;

  {
     tmp = zero;
     for ( int i = 0; i < NSIZE; ++i) {
         tmp += matc[i][i];
     }
  }
  printf("Elapsed (sec) %10.4f,# of nitr. = %d, trace =  %23.12e\n",elp, NITR, tmp);
  /*====================================================================*/

  printf("--------------------\n");
  return EXIT_SUCCESS;
}

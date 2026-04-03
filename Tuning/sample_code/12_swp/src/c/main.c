/* Copyright 2024 Research Organization for Information Science and Technology */
/* ==================================================================== 
 * Example of software pipelining 
 *
 * Authors    : Yukihiro Ota (yota@rist.or.jp)
 * Last update: 15th Feb. 2024
 * ==================================================================*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "mykernel.h"
#include "timer.h"

#define NITR_MAX 40000
#define NDIM_DEFAULT 120
#define NDIM_MIN 10
#define NDIM_MAX 2000

int main (int argc, char **argv)
{
   int ndim;
   int mvec;
   int icon;
   int NITR;

   double elp0, elp[2];
   double vscale;
   double *A;
   double *Q;

   if ( argc != 2 ) {
      fprintf(stdout,"[usage] run.x (arg1) \n"
                     "   (arg1): Dimension of vector (int)\n");
      return EXIT_SUCCESS; 
   }

   ndim = atoi(argv[1]);
   if ( ndim <= 0 ) {
      ndim = NDIM_DEFAULT;
   }
   if ( ndim < NDIM_MIN ) {
      fprintf(stderr,"[ERROR] (arg1) must be larger than %d\n", NDIM_MIN);
      return EXIT_FAILURE;
   }
   if ( ndim > NDIM_MAX ) {
      fprintf(stderr,"[ERROR] (arg1) must be smaller than %d\n", NDIM_MAX);
      return EXIT_FAILURE;
   }

   /* memory allocation */
   A = create_dp_1d_array( ndim );  
   Q = create_dp_1d_array( ndim*ndim );  

   /* initialization */
   vscale = (double)ndim;
   vscale = 1.0 / (vscale*vscale);

   for ( int i = 0; i < ndim; ++i ) {
      for (int j = 0; j < ndim; ++j ) {
         int ij = j + i*ndim;
         Q[ij] = 0.0;
      }
   }

   unsigned int SEED = 409101;

   for ( int i = 0; i < ndim; ++i ) {

      A[i] = vscale;

      for (int j = 0; j <= i; ++j ) {
         int ij = j + i*ndim;
         Q[ij] = ((double)rand() / RAND_MAX) * vscale;
      }

   }
 
   if ( ndim < 300 ) {
      NITR = NITR_MAX;
   } else if ( ndim < 600 ) {
      NITR = NITR_MAX/2;
   } else if ( ndim < 1200 ) {
      NITR = NITR_MAX/4;
   } else {
      NITR = 1000;
   }

   fprintf(stdout,"> Legends in output\n"
                  "> * kernel          : Types of kernels\n"
                  "> * ndim            : Dimension of vector (innermost-loop length)\n"
                  "> * mvec            : Outermost-loop length; target for software pipelining\n"
                  "> * NITR            : Number of iterations\n"
                  "> * Elapsed_time_sec: Elapsed time (sec.)\n"
                  "> * dummy           : Dummy output (no information)\n");

   fprintf(stdout,"%16s %8s %8s %8s %18s %8s\n", 
           "kernel", "ndim", "mvec", "NITR", "Elapsed_time_sec", "dummy");

   /* =========================
    * large mvec 
    * ====================== */
   mvec = ndim - 1;

   /* not software pipelining */
   elp0 = get_elp_time();

   icon = 0.0;
   for ( int itr = 0; itr < NITR; ++itr) {
      coremgs(A, Q, ndim, mvec); 
      icon += dummy(A, ndim);

      for ( int i = 0; i < ndim; ++i ) {
          A[i] = vscale;
      }
   }

   elp[0] = get_elp_time() - elp0;

   /* software pipelining */
   elp0 = get_elp_time();

   icon = 0.0;
   for ( int itr = 0; itr < NITR; ++itr) {
      coremgs_swp(A, Q, ndim, mvec); 
      icon += dummy(A, ndim);

      for ( int i = 0; i < ndim; ++i ) {
          A[i] = vscale;
      }
   }

   elp[1] = get_elp_time() - elp0;

   fprintf(stdout,"%16s %8d %8d %8d %18.3f %8d\n",
           "coremgs", ndim, mvec, NITR, elp[0], icon);
   fprintf(stdout,"%16s %8d %8d %8d %18.3f %8d\n",
           "coremgs_spw", ndim, mvec, NITR, elp[1], icon);

   /* =========================
    * medium mvec 
    * ====================== */
   mvec = ndim/2;

   /* not software pipelining */
   elp0 = get_elp_time();

   icon = 0.0;
   for ( int itr = 0; itr < NITR; ++itr) {
      coremgs(A, Q, ndim, mvec); 
      icon += dummy(A, ndim);

      for ( int i = 0; i < ndim; ++i ) {
          A[i] = vscale;
      }
   }

   elp[0] = get_elp_time() - elp0;

   /* software pipelining */
   elp0 = get_elp_time();

   icon = 0.0;
   for ( int itr = 0; itr < NITR; ++itr) {
      coremgs_swp(A, Q, ndim, mvec); 
      icon += dummy(A, ndim);

      for ( int i = 0; i < ndim; ++i ) {
          A[i] = vscale;
      }
   }

   elp[1] = get_elp_time() - elp0;

   fprintf(stdout,"%16s %8d %8d %8d %18.3f %8d\n",
           "coremgs", ndim, mvec, NITR, elp[0], icon);
   fprintf(stdout,"%16s %8d %8d %8d %18.3f %8d\n",
           "coremgs_spw", ndim, mvec, NITR, elp[1], icon);

   /* =========================
    * small mvec 
    * ====================== */
   mvec = ndim / 4;

   /* not software pipelining */
   elp0 = get_elp_time();

   icon = 0.0;
   for ( int itr = 0; itr < NITR; ++itr) {
      coremgs(A, Q, ndim, mvec); 
      icon += dummy(A, ndim);

      for ( int i = 0; i < ndim; ++i ) {
          A[i] = vscale;
      }
   }

   elp[0] = get_elp_time() - elp0;

   /* software pipelining */
   elp0 = get_elp_time();

   icon = 0.0;
   for ( int itr = 0; itr < NITR; ++itr) {
      coremgs_swp(A, Q, ndim, mvec); 
      icon += dummy(A, ndim);

      for ( int i = 0; i < ndim; ++i ) {
          A[i] = vscale;
      }
   }

   elp[1] = get_elp_time() - elp0;

   fprintf(stdout,"%16s %8d %8d %8d %18.3f %8d\n",
           "coremgs", ndim, mvec, NITR, elp[0], icon);
   fprintf(stdout,"%16s %8d %8d %8d %18.3f %8d\n",
           "coremgs_spw", ndim, mvec, NITR, elp[1], icon);

   /* finalization */
   release_dp_1d_array( A );
   release_dp_1d_array( Q );

   return EXIT_SUCCESS;
}

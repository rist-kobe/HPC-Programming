/* Copyright 2025 Research Organization for Information Science and Technology */
/* Reference
   * Tom Deakin and Timothy G. Mattson, Programming your GPU with OpenMP: 
     Performance Portability for GPUs (MIT, 2023) Chap.9.
*/
/*====================================================================
 * cuBLAS
 * https://docs.nvidia.com/cuda/cublas/index.htmludd
 *====================================================================*/
#include <cuda_runtime.h>
#include <cublas_v2.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

#include "mykernel.h"
#include "timer.h"

#define NITR_MAX 20000

extern void mmp_cuda_wrapper(int, double *, double *, double *);

int main (int argc, char *argv[])
{
  int NSIZE, NITR;
  int icon;
  double zero = 0.0;
  double one  = 1.0;
  double tmp, diff, elp0, elp, gflop, gflops, elp1, elp2;
  double *mata, *matb, *matc, *matcc;

  cublasStatus_t cublas_stat;
  cublasHandle_t cublas_handle;

  if ( argc != 2 ) {
     printf("[usage] run.x (arg1) \n");
     printf("        (arg1): matrix dimension (integer)\n");
     return EXIT_SUCCESS;
  }

  NSIZE = atoi(argv[1]);

  mata = (double *)malloc( NSIZE*NSIZE*sizeof(double) );
  matb = (double *)malloc( NSIZE*NSIZE*sizeof(double) );
  matc = (double *)malloc( NSIZE*NSIZE*sizeof(double) );
  matcc= (double *)malloc( NSIZE*NSIZE*sizeof(double) );

  tmp = (double)NSIZE;
  gflop = 2.0 * tmp * tmp * tmp * 1.0e-9;

  NITR = NITR_MAX / NSIZE;
  if ( NITR < 10 ) {
     NITR = 50;
  } else if ( NITR < 100 ) {
     NITR = 50 * NITR;
  } else {
     NITR = NITR_MAX;
  }

  {
     unsigned int seed = 409101;
     tmp = 1.0 / NSIZE;
     srand( seed );
     for ( int i = 0; i < NSIZE; ++i ) {
     for ( int j = 0; j < NSIZE; ++j ) {
         int ji = j + NSIZE*i;
         mata[ji] = (2.0*(double)rand()/RAND_MAX - one) * tmp;
         matb[ji] = (2.0*(double)rand()/RAND_MAX - one) * tmp;
	 matc[ji] = 0.0;
     }}
  }

  cublas_stat = cublasCreate(&cublas_handle);
  if ( cublas_stat != CUBLAS_STATUS_SUCCESS) {
     fprintf (stderr, "CUBLAS initialization failed\n");
     return EXIT_FAILURE;
  }

  printf("%-24s %10s %10s %10s %10s %10s %23s %23s\n", 
         "kernel", "NSIZE", "NITR", "ELP_H+D", "ELP", "GFlop/s", "trace", "RMSE");
/*======================================================================
 * [Simple implementation]
 *====================================================================*/
  elp0 = get_elp_time();
  
  icon = 0;
  for ( int it = 0; it < NITR; ++it) {
     mmp_simple(NSIZE, matc, mata, matb);
     icon += dummy(it, NSIZE, matc);
  }

  elp  = get_elp_time() - elp0;

  tmp = zero;
  for ( int i = 0; i < NSIZE; ++i) {
     int ji = i + NSIZE*i;
     tmp += matc[ji];
  }
 
  gflops = (gflop*NITR) / elp;

  elp2 = elp;

  diff = zero;

  printf("%-24s %10d %10d %10.3f %10.3f %10.3f %23.12e %23.12e\n", 
     "simple", NSIZE, NITR, elp2, elp, gflops, tmp, diff);
  
  for ( int i = 0; i < NSIZE; ++i ) {
  for ( int j = 0; j < NSIZE; ++j ) {
      int ji = j + NSIZE*i;
      matcc[ji] = matc[ji];
      matc[ji] = 0.0;
  }}
/*======================================================================
 * [warm-up]
 *====================================================================*/
  #pragma acc data copyin(mata[0:NSIZE*NSIZE], matb[0:NSIZE*NSIZE]) \
  copy(matc[0:NSIZE*NSIZE])
  {
  icon = 0;
  for ( int it = 0; it < 3; ++it) {
     mmp_simple_omptarget(NSIZE, matc, mata, matb);
     icon += dummy(it, NSIZE, matc);
  }
  } /* acc data */

  for ( int i = 0; i < NSIZE; ++i ) {
  for ( int j = 0; j < NSIZE; ++j ) {
      int ji = j + NSIZE*i;
      matc[ji] = 0.0;
  }}
/*======================================================================
 * [Simple implementation: OpenMP Target]
 *====================================================================*/
  elp0 = get_elp_time();

  #pragma omp target data map(to:mata[0:NSIZE*NSIZE], matb[0:NSIZE*NSIZE]) \
  map(tofrom:matc[0:NSIZE*NSIZE])
  {
  elp1 = get_elp_time();
  
  icon = 0;
  for ( int it = 0; it < NITR; ++it) {
     mmp_simple_omptarget(NSIZE, matc, mata, matb);
     icon += dummy(it, NSIZE, matc);
  }

  elp = get_elp_time() - elp1;
  } /* omp target data */

  elp2 = get_elp_time() - elp0;

  tmp = zero;
  for ( int i = 0; i < NSIZE; ++i) {
     int ji = i + NSIZE*i;
     tmp += matc[ji];
  }
 
  gflops = (gflop*NITR) / elp;

  diff = 0.0;
  for ( int i = 0; i < NSIZE; ++i ) {
  for ( int j = 0; j < NSIZE; ++j ) {
      int ji = j + NSIZE*i;
      double dd = matc[ji] - matcc[ji];
      diff += dd*dd;
  }}
  diff = sqrt(diff)/NSIZE;

  printf("%-24s %10d %10d %10.3f %10.3f %10.3f %23.12e %23.12e\n", 
     "simple_omptarget", NSIZE, NITR, elp2, elp, gflops, tmp, diff);

  for ( int i = 0; i < NSIZE; ++i ) {
  for ( int j = 0; j < NSIZE; ++j ) {
      int ji = j + NSIZE*i;
      matc[ji] = 0.0;
  }}
/*======================================================================
 * [Simple implementation: OpenMP Target hierarchical 1
 *====================================================================*/
  elp0 = get_elp_time();

  #pragma omp target data map(to:mata[0:NSIZE*NSIZE], matb[0:NSIZE*NSIZE]) \
  map(tofrom:matc[0:NSIZE*NSIZE])
  {
  elp1 = get_elp_time();
  
  icon = 0;
  for ( int it = 0; it < NITR; ++it) {
     mmp_simple_omptarget_h1(NSIZE, matc, mata, matb);
     icon += dummy(it, NSIZE, matc);
  }

  elp = get_elp_time() - elp1;
  } /* omp target data */

  elp2 = get_elp_time() - elp0;

  tmp = zero;
  for ( int i = 0; i < NSIZE; ++i) {
     int ji = i + NSIZE*i;
     tmp += matc[ji];
  }
 
  gflops = (gflop*NITR) / elp;

  diff = 0.0;
  for ( int i = 0; i < NSIZE; ++i ) {
  for ( int j = 0; j < NSIZE; ++j ) {
      int ji = j + NSIZE*i;
      double dd = matc[ji] - matcc[ji];
      diff += dd*dd;
  }}
  diff = sqrt(diff)/NSIZE;

  printf("%-24s %10d %10d %10.3f %10.3f %10.3f %23.12e %23.12e\n", 
     "simple_omptarget_h1", NSIZE, NITR, elp2, elp, gflops, tmp, diff);

  for ( int i = 0; i < NSIZE; ++i ) {
  for ( int j = 0; j < NSIZE; ++j ) {
      int ji = j + NSIZE*i;
      matc[ji] = 0.0;
  }}
/*======================================================================
 * [Simple implementation: OpenMP Target hierarchical 2
 *====================================================================*/
  elp0 = get_elp_time();

  #pragma omp target data map(to:mata[0:NSIZE*NSIZE], matb[0:NSIZE*NSIZE]) \
  map(tofrom:matc[0:NSIZE*NSIZE])
  {
  elp1 = get_elp_time();
  
  icon = 0;
  for ( int it = 0; it < NITR; ++it) {
     mmp_simple_omptarget_h2(NSIZE, matc, mata, matb);
     icon += dummy(it, NSIZE, matc);
  }

  elp = get_elp_time() - elp1;
  } /* omp target data */

  elp2 = get_elp_time() - elp0;

  tmp = zero;
  for ( int i = 0; i < NSIZE; ++i) {
     int ji = i + NSIZE*i;
     tmp += matc[ji];
  }
 
  gflops = (gflop*NITR) / elp;

  diff = 0.0;
  for ( int i = 0; i < NSIZE; ++i ) {
  for ( int j = 0; j < NSIZE; ++j ) {
      int ji = j + NSIZE*i;
      double dd = matc[ji] - matcc[ji];
      diff += dd*dd;
  }}
  diff = sqrt(diff)/NSIZE;

  printf("%-24s %10d %10d %10.3f %10.3f %10.3f %23.12e %23.12e\n", 
     "simple_omptarget_h2", NSIZE, NITR, elp2, elp, gflops, tmp, diff);

  for ( int i = 0; i < NSIZE; ++i ) {
  for ( int j = 0; j < NSIZE; ++j ) {
      int ji = j + NSIZE*i;
      matc[ji] = 0.0;
  }}
/*======================================================================
 * [My CUDA implementation]
 *====================================================================*/
  elp0 = get_elp_time();

  #pragma omp target data map(to:mata[0:NSIZE*NSIZE],matb[0:NSIZE*NSIZE]) \
  map(tofrom:matc[0:NSIZE*NSIZE])
  {
  elp1 = get_elp_time();

  icon = 0;
  for ( int it = 0; it < NITR; ++it) {
     #pragma omp target data use_device_ptr(mata, matb, matc)
     {
        mmp_cuda_wrapper(NSIZE, matc, mata, matb);
     } 
     icon += dummy(it, NSIZE, matc);
  }

  elp  = get_elp_time() - elp1;
  } /* omp target data */

  elp2 = get_elp_time() - elp0;

  tmp = zero;
  for ( int i = 0; i < NSIZE; ++i) {
     int ji = i + NSIZE*i;
     tmp += matc[ji];
  }
 
  gflops = (gflop*NITR) / elp;

  diff = 0.0;
  for ( int i = 0; i < NSIZE; ++i ) {
  for ( int j = 0; j < NSIZE; ++j ) {
      int ji = j + NSIZE*i;
      double dd = matc[ji] - matcc[ji];
      diff += dd*dd;
  }}
  diff = sqrt(diff)/NSIZE;

  printf("%-24s %10d %10d %10.3f %10.3f %10.3f %23.12e %23.12e\n", 
     "my_CUDA_impl", NSIZE, NITR, elp2, elp, gflops, tmp, diff);

  for ( int i = 0; i < NSIZE; ++i ) {
  for ( int j = 0; j < NSIZE; ++j ) {
      int ji = j + NSIZE*i;
      matc[ji] = 0.0;
  }}
/*======================================================================
 * [DGEMM in cuBLAS]
 * Note: cuBLAS is implemented by column-major order 
 *====================================================================*/
  elp0 = get_elp_time();

  double *matc_trans = (double *) malloc ( NSIZE*NSIZE*sizeof(double) );

  for ( int i = 0; i < NSIZE; ++i ) {
  for ( int j = 0; j < NSIZE; ++j ) {
      int ji = j + NSIZE*i;
      int ij = i + NSIZE*j;
      matc_trans[ji] = matc[ij];
  }}

  #pragma omp target data map(to:mata[0:NSIZE*NSIZE],matb[0:NSIZE*NSIZE]) \
  map(tofrom:matc_trans[0:NSIZE*NSIZE]) 
  {
  elp1 = get_elp_time();

  icon = 0;
  for ( int it = 0; it < NITR; ++it) {
     #pragma omp target data use_device_ptr(mata, matb, matc_trans)
     {
        cublasDgemm(cublas_handle, CUBLAS_OP_T, CUBLAS_OP_T, NSIZE, NSIZE, NSIZE,
           &one, mata, NSIZE, matb, NSIZE, &zero, matc_trans, NSIZE);
     } 

     const cudaError_t error = cudaDeviceSynchronize();
     if ( error != cudaSuccess) {
        exit(1);
     }
     icon += dummy(it, NSIZE, matc);
  }

  elp  = get_elp_time() - elp1;
  } /* omp target data */

  for ( int i = 0; i < NSIZE; ++i ) {
  for ( int j = 0; j < NSIZE; ++j ) {
      int ji = j + NSIZE*i;
      int ij = i + NSIZE*j;
      matc[ji] = matc_trans[ij];
  }}

  free( matc_trans );

  elp2 = get_elp_time() - elp0;

  tmp = zero;
  for ( int i = 0; i < NSIZE; ++i) {
     int ji = i + NSIZE*i;
     tmp += matc[ji];
  }
 
  gflops = (gflop*NITR) / elp;

  diff = 0.0;
  for ( int i = 0; i < NSIZE; ++i ) {
  for ( int j = 0; j < NSIZE; ++j ) {
      int ji = j + NSIZE*i;
      double dd = matc[ji] - matcc[ji];
      diff += dd*dd;
  }}
  diff = sqrt(diff)/NSIZE;

  printf("%-24s %10d %10d %10.3f %10.3f %10.3f %23.12e %23.12e\n", 
     "DGEMM_cuBLAS", NSIZE, NITR, elp2, elp, gflops, tmp, diff);

  for ( int i = 0; i < NSIZE; ++i ) {
  for ( int j = 0; j < NSIZE; ++j ) {
      int ji = j + NSIZE*i;
      matc[ji] = 0.0;
  }}

  free( mata );
  free( matb );
  free( matc );
  free( matcc );

  return EXIT_SUCCESS;
}

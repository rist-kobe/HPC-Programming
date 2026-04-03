/* Copyright 2024 Research Organization for Information Science and Technology */
/*-----------------------------------------------------
! Reduce load imbalance
! Author:  Yukihiro Ota (yota@rist.or.jp)
! Description:  
!          This is a miniature of calculating newtonian 
!          pair force in MD simulation. The original  
!          idea was shown by D. Yamaki and Y. Aoyama at RIST.
!-----------------------------------------------------*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NSIZE 80000

int main(int argc, char **argv) {
  int i, j, ns; 
  double f[NSIZE], x[NSIZE], fij, t, ts;

  for ( i = 0; i<NSIZE; ++i) {
     x[i] = ((double)rand()) / RAND_MAX;
  }

  ns = NSIZE;

  /* Serial run                                                     */
  for ( i =0; i<NSIZE; ++i ) { 
     f[i] = 0.0;
  }

  ts = omp_get_wtime();
  for ( i = 0; i < NSIZE-1; ++i ) {
     for ( j = i+1; j < NSIZE; ++j ) {
        fij = 1.0 / fabs(x[j]-x[i]);
        f[i] = f[i] + fij;
        f[j] = f[j] - fij;
     }
  } 
  ts = omp_get_wtime() - ts;
  printf("[Serial] Elapsed time (sec)                = %9.3f\n", ts);

  /* Parallel run: static scheduling                                */
  for ( i =0; i<NSIZE; ++i ) { 
     f[i] = 0.0;
  }

  t = omp_get_wtime();
  #pragma omp parallel for schedule(static) private(fij) \
  reduction(+:f[:ns])
  for ( i = 0; i < NSIZE-1; ++i ) {
     for ( j = i+1; j < NSIZE; ++j ) {
        fij = 1.0 / fabs(x[j]-x[i]);
        f[i] = f[i] + fij;
        f[j] = f[j] - fij;
     }
  } 
  t = omp_get_wtime() - t;
  printf("[OMP static] Elapsed time (sec)            = %9.3f, \
  Speedup: %7.1f\n", t, ts/t);

  /* Parallel run: static scheduling with chunk size 1              */
  for ( i =0; i<NSIZE; ++i ) { 
     f[i] = 0.0;
  }

  t = omp_get_wtime();
  #pragma omp parallel for schedule(static,1) private(fij) \
  reduction(+:f[:ns])
  for ( i = 0; i < NSIZE-1; ++i ) {
     for ( j = i+1; j < NSIZE; ++j ) {
        fij = 1.0 / (x[j]-x[i]);
        f[i] = f[i] + fij;
        f[j] = f[j] - fij;
     }
  } 
  t = omp_get_wtime() - t;
  printf("[OMP static chunk 1] Elapsed time (sec)    = %9.3f, \
  Speedup: %7.1f\n", t, ts/t);

  /* Parallel run: dynamical scheduling with chunk size 1           */
  for ( i =0; i<NSIZE; ++i ) { 
     f[i] = 0.0;
  }

  t = omp_get_wtime();
  #pragma omp parallel for schedule(dynamic,1) private(fij) \
  reduction(+:f[:ns])
  for ( i = 0; i < NSIZE-1; ++i ) {
     for ( j = i+1; j < NSIZE; ++j ) {
        fij = 1.0 / fabs(x[j]-x[i]);
        f[i] = f[i] + fij;
        f[j] = f[j] - fij;
     }
  } 
  t = omp_get_wtime() - t;
  printf("[OMP dynamic chunk 1] Elapsed time (sec)   = %9.3f, \
  Speedup: %7.1f\n", t, ts/t);

  return EXIT_SUCCESS;
}

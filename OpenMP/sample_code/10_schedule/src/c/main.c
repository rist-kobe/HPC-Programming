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

/* Pair-force kernel: serial reference                              */
void force_serial(const double *x, double *f) {
  int i, j;
  double fij;

  for ( i = 0; i < NSIZE-1; ++i ) {
     for ( j = i+1; j < NSIZE; ++j ) {
        fij = 1.0 / fabs(x[j]-x[i]);
        f[i] = f[i] + fij;
        f[j] = f[j] - fij;
     }
  }
}

/* Pair-force kernel: static scheduling                             */
void force_static(const double *x, double *f) {
  int i, j;
  double fij;

  #pragma omp parallel for schedule(static) private(j, fij) \
  reduction(+:f[:NSIZE])
  for ( i = 0; i < NSIZE-1; ++i ) {
     for ( j = i+1; j < NSIZE; ++j ) {
        fij = 1.0 / fabs(x[j]-x[i]);
        f[i] = f[i] + fij;
        f[j] = f[j] - fij;
     }
  }
}

/* Pair-force kernel: static scheduling with chunk size 1           */
void force_static1(const double *x, double *f) {
  int i, j;
  double fij;

  #pragma omp parallel for schedule(static,1) private(j, fij) \
  reduction(+:f[:NSIZE])
  for ( i = 0; i < NSIZE-1; ++i ) {
     for ( j = i+1; j < NSIZE; ++j ) {
        fij = 1.0 / fabs(x[j]-x[i]);
        f[i] = f[i] + fij;
        f[j] = f[j] - fij;
     }
  }
}

/* Pair-force kernel: dynamical scheduling with chunk size 1        */
void force_dynamic1(const double *x, double *f) {
  int i, j;
  double fij;

  #pragma omp parallel for schedule(dynamic,1) private(j, fij) \
  reduction(+:f[:NSIZE])
  for ( i = 0; i < NSIZE-1; ++i ) {
     for ( j = i+1; j < NSIZE; ++j ) {
        fij = 1.0 / fabs(x[j]-x[i]);
        f[i] = f[i] + fij;
        f[j] = f[j] - fij;
     }
  }
}

/* Pair-force kernel: guided scheduling                             */
void force_guided(const double *x, double *f) {
  int i, j;
  double fij;

  #pragma omp parallel for schedule(guided) private(j, fij) \
  reduction(+:f[:NSIZE])
  for ( i = 0; i < NSIZE-1; ++i ) {
     for ( j = i+1; j < NSIZE; ++j ) {
        fij = 1.0 / fabs(x[j]-x[i]);
        f[i] = f[i] + fij;
        f[j] = f[j] - fij;
     }
  }
}

/* Maximum absolute difference from the serial reference            */
double max_diff(const double *fref, const double *f) {
  int i;
  double d, dmax = 0.0;

  for ( i = 0; i < NSIZE; ++i ) {
     d = fabs(f[i]-fref[i]);
     if ( d > dmax ) dmax = d;
  }
  return dmax;
}

int main(int argc, char **argv) {
  int i;
  double f[NSIZE], fref[NSIZE], x[NSIZE], t, ts;

  for ( i = 0; i<NSIZE; ++i) {
     x[i] = ((double)rand()) / RAND_MAX;
  }

  printf("Number of threads                          = %9d\n",
  omp_get_max_threads());

  /* Warm-up: create the thread team before the timed runs          */
  #pragma omp parallel
  {
     /* Do nothing */
  }

  /* Serial run                                                     */
  for ( i =0; i<NSIZE; ++i ) { 
     fref[i] = 0.0;
  }

  ts = omp_get_wtime();
  force_serial(x, fref);
  ts = omp_get_wtime() - ts;
  printf("[Serial] Elapsed time (sec)                = %9.3f\n", ts);

  /* Parallel run: static scheduling                                */
  for ( i =0; i<NSIZE; ++i ) { 
     f[i] = 0.0;
  }

  t = omp_get_wtime();
  force_static(x, f);
  t = omp_get_wtime() - t;
  printf("[OMP static] Elapsed time (sec)            = %9.3f,"
  " Speedup: %7.1f, Max diff: %9.2e\n", t, ts/t, max_diff(fref, f));

  /* Parallel run: static scheduling with chunk size 1              */
  for ( i =0; i<NSIZE; ++i ) { 
     f[i] = 0.0;
  }

  t = omp_get_wtime();
  force_static1(x, f);
  t = omp_get_wtime() - t;
  printf("[OMP static chunk 1] Elapsed time (sec)    = %9.3f,"
  " Speedup: %7.1f, Max diff: %9.2e\n", t, ts/t, max_diff(fref, f));

  /* Parallel run: dynamical scheduling with chunk size 1           */
  for ( i =0; i<NSIZE; ++i ) { 
     f[i] = 0.0;
  }

  t = omp_get_wtime();
  force_dynamic1(x, f);
  t = omp_get_wtime() - t;
  printf("[OMP dynamic chunk 1] Elapsed time (sec)   = %9.3f,"
  " Speedup: %7.1f, Max diff: %9.2e\n", t, ts/t, max_diff(fref, f));

  /* Parallel run: guided scheduling                                */
  for ( i =0; i<NSIZE; ++i ) { 
     f[i] = 0.0;
  }

  t = omp_get_wtime();
  force_guided(x, f);
  t = omp_get_wtime() - t;
  printf("[OMP guided] Elapsed time (sec)            = %9.3f,"
  " Speedup: %7.1f, Max diff: %9.2e\n", t, ts/t, max_diff(fref, f));

  return EXIT_SUCCESS;
}

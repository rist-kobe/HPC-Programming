/* Copyright 2025 Research Organization for Information Science and Technology */
#include "mykernel.h"
/*=========================================================================
 * Simple implementation
 * + Memory access pattern 
 *       stride  : non-contiguous access with stride > 1
 *       cont    : contiguous access (stride 1 access)
 *       register: on register 
 * ====================================================
 * loop depth |     mc         ma         mb 
 *  i         | stride      stride      register 
 *  k         | register    cont        stride  
 *  j         | cont        register    cont 
 *=======================================================================*/
void mmp_simple (const int ns, double * restrict mc, 
  double * restrict ma, double * restrict mb)
{
  int i, j, k, ji;

  for ( ji = 0; ji < ns*ns; ++ji ) {
     mc[ji] = 0.0; 
  }

  for ( i = 0; i < ns; ++i ) {
  for ( k = 0; k < ns; ++k ) {
      int ki = k + ns*i;
  for ( j = 0; j < ns; ++j ) {
      int ji = j + ns*i;
      int jk = j + ns*k;
      mc[ji] += ma[ki]*mb[jk]; /* mc[i][j] += ma[i][k]*mb[k][j]; */
  }}}
}

/*=========================================================================
 * Simple implementation with OpenMP Target
 * + Memory access pattern 
 *       stride  : non-contiguous access with stride > 1
 *       cont    : contiguous access (stride 1 access)
 *       register: on register 
 * ====================================================
 * loop depth |     mc         ma         mb 
 *  i         | stride      stride      register 
 *  k         | register    cont        stride  
 *  j         | cont        register    cont 
 *=======================================================================*/
void mmp_simple_omptarget (const int ns, double * restrict mc, 
  double * restrict ma, double * restrict mb)
{
  #pragma omp target map(present:mc[0:ns*ns])
  #pragma omp loop  
  for ( int ji = 0; ji < ns*ns; ++ji ) {
     mc[ji] = 0.0; 
  }
 
  #pragma omp target map(present:ma[0:ns*ns],mb[0:ns*ns],mc[0:ns*ns])
  #pragma omp loop collapse(2) 
  for ( int i = 0; i < ns; ++i ) {
  for ( int j = 0; j < ns; ++j ) {
     double mctmp = 0.0;
     for ( int k = 0; k < ns; ++k ) {
        mctmp += ma[k + i*ns]*mb[j + k*ns]; /* mctmp += ma[i][k]*mb[k][j] */
     }
     mc[j + ns*i] += mctmp; /* mc[i][j] */
  }}
}

/*=========================================================================
 * Simple implementation with OpenMP Target: hierarchical parallelism 1
 * + Memory access pattern 
 *       stride  : non-contiguous access with stride > 1
 *       cont    : contiguous access (stride 1 access)
 *       register: on register 
 * ====================================================
 * loop depth |     mc         ma         mb 
 *  i         | stride      stride      register 
 *  k         | register    cont        stride  
 *  j         | cont        register    cont 
 *=======================================================================*/
void mmp_simple_omptarget_h1 (const int ns, double * restrict mc, 
  double * restrict ma, double * restrict mb)
{
  #pragma omp target map(present:mc[0:ns*ns])
  #pragma omp loop 
  for ( int ji = 0; ji < ns*ns; ++ji ) {
     mc[ji] = 0.0; 
  }
 
  #pragma omp target map(present:ma[0:ns*ns],mb[0:ns*ns],mc[0:ns*ns])
  #pragma omp teams distribute 
  for ( int i = 0; i < ns; ++i ) {
  #pragma omp parallel for 
  for ( int j = 0; j < ns; ++j ) {
     double mctmp = 0.0;
     for ( int k = 0; k < ns; ++k ) {
        mctmp += ma[k + i*ns]*mb[j + k*ns]; /* mctmp += ma[i][k]*mb[k][j] */
     }
     mc[j + ns*i] += mctmp; /* mc[i][j] */
  }}
}

/*=========================================================================
 * Simple implementation with OpenMP Target: hierarchical parallelism 2
 * + Memory access pattern 
 *       stride  : non-contiguous access with stride > 1
 *       cont    : contiguous access (stride 1 access)
 *       register: on register 
 * ====================================================
 * loop depth |     mc         ma         mb 
 *  i         | stride      stride      register 
 *  k         | register    cont        stride  
 *  j         | cont        register    cont 
 *=======================================================================*/
void mmp_simple_omptarget_h2 (const int ns, double * restrict mc, 
  double * restrict ma, double * restrict mb)
{
  #pragma omp target map(present:mc[0:ns*ns])
  #pragma omp loop 
  for ( int ji = 0; ji < ns*ns; ++ji ) {
     mc[ji] = 0.0; 
  }
 
  #pragma omp target map(present:ma[0:ns*ns],mb[0:ns*ns],mc[0:ns*ns])
  #pragma omp teams loop 
  for ( int i = 0; i < ns; ++i ) {
  #pragma omp loop 
  for ( int j = 0; j < ns; ++j ) {
     double mctmp = 0.0;
     for ( int k = 0; k < ns; ++k ) {
        mctmp += ma[k + i*ns]*mb[j + k*ns]; /* mctmp += ma[i][k]*mb[k][j] */
     }
     mc[j + ns*i] += mctmp; /* mc[i][j] */
  }}
}

/*=========================================================================
 * Simple implementation with loop blocking
 * + Memory access pattern 
 *       stride  : non-contiguous access with stride > 1
 *       cont    : contiguous access (stride 1 access)
 *       register: on register 
 * ====================================================
 * loop depth |     mc         ma         mb 
 *  i         | stride      stride      register 
 *  k         | register    cont        stride  
 *  j         | cont        register    cont 
 *=======================================================================*/
void mmp_simple_blk (const int ns, const int nbk1, const int nbk2, 
  double * restrict mc, double * restrict ma, double * restrict mb)
{
  int i, j, k, ji, jj, kk;

  for ( ji = 0; ji < ns*ns; ++ji ) {
     mc[ji] = 0.0; 
  }

  for ( kk = 0; kk < ns; kk += nbk2 ) {
  for ( jj = 0; jj < ns; jj += nbk1 ) {

  for ( i = 0; i < ns; ++i ) {
     for ( k = kk; k < MIN(ns,kk+nbk2); ++k ) {
        int ki = k + i*ns;
     for ( j = jj; j < MIN(ns,jj+nbk1); ++j ) {
        int ji = j + ns*i;
	int jk = j + ns*k;
        mc[ji] += ma[ki]*mb[jk]; /* mc[i][j] += ma[i][k]*mb[k][j];*/
     }}
  }
  
  }}
}

/*=========================================================================
 * Simple implementation
 * use of variable arrays in C99 
 * note: doublle A[][ns] is equal to double (*A)[ns]  
 * + Memory access pattern 
 *       stride  : non-contiguous access with stride > 1
 *       cont    : contiguous access (stride 1 access)
 *       register: on register 
 * ====================================================
 * loop depth |     mc         ma         mb 
 *  i         | stride      stride      register 
 *  k         | register    cont        stride  
 *  j         | cont        register    cont 
 *=======================================================================*/
void mmp_simple_va (const int ns, double mc[restrict][ns], 
  double ma[restrict][ns], double mb[restrict][ns])
{
  int i, j, k;

  for ( i = 0; i < ns; ++i ) {
  for ( j = 0; j < ns; ++j ) {
      mc[i][j] = 0.0;  
  }}

  for ( i = 0; i < ns; ++i ) {
  for ( k = 0; k < ns; ++k ) {
  for ( j = 0; j < ns; ++j ) {
      mc[i][j] += ma[i][k]*mb[k][j];
  }}}
}

/*=========================================================================
 * Simple implementation with loop blocking
 * use of variable arrays in C99 
 * note: doublle A[][ns] is equal to double (*A)[ns]  *
 * + Memory access pattern 
 *       stride  : non-contiguous access with stride > 1
 *       cont    : contiguous access (stride 1 access)
 *       register: on register 
 * ====================================================
 * loop depth |     mc         ma         mb 
 *  i         | stride      stride      register 
 *  k         | register    cont        stride  
 *  j         | cont        register    cont 
 *=======================================================================*/
void mmp_simple_blk_va (const int ns, const int nbk1, const int nbk2, 
  double mc[restrict][ns], double ma[restrict][ns], double mb[restrict][ns])
{
  int i, j, k, jj, kk;

  for ( i = 0; i < ns; ++i ) {
  for ( j = 0; j < ns; ++j ) {
      mc[i][j] = 0.0;  
  }}

  for ( kk = 0; kk < ns; kk += nbk2 ) {
  for ( jj = 0; jj < ns; jj += nbk1 ) {

  for ( i = 0; i < ns; ++i ) {
     for ( k = kk; k < MIN(ns,kk+nbk2); ++k ) {
     for ( j = jj; j < MIN(ns,jj+nbk1); ++j ) {
        mc[i][j] += ma[i][k]*mb[k][j];
     }}
  }
  
  }}
}

/*=========================================================================
 * dummy
 *=======================================================================*/
int dummy (const int it, const int n, double * restrict mat) 
{
   int icon = 0;
   return icon;
}

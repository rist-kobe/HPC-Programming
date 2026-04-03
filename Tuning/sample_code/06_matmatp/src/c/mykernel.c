/* Copyright 2024 Research Organization for Information Science and Technology */
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
void mmp_simple(const int ns, double mc[restrict][ns], 
  double ma[restrict][ns], double mb[restrict][ns])
/* use of variable arrays in C99 
 * note: double A[][ns] is equal to double (*A)[ns]  */
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
void mmp_simple_blk(const int ns, const int nbk1, const int nbk2, 
  double mc[restrict][ns], double ma[restrict][ns], double mb[restrict][ns])
/* use of variable arrays in C99 
 * note: double A[][ns] is equal to double (*A)[ns]  */
{
  int i, j, k, jj, kk;

  for ( i = 0; i < ns; ++i ) {
  for ( j = 0; j < ns; ++j ) {
     mc[i][j] = 0.0;  
  }}

  /* (cache size/core) > nbk1*nbk2 + nbk1 + nbk2        */
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
 * Exchanging loop, Transpose, and Blocking (two inner loops) 
 * + Memory access pattern 
 *       stride  : non-contiguous access with stride > 1
 *       cont    : contiguous access (stride 1 access)
 *       register: on register 
 * ====================================================
 * loop depth |     mc         ma         work 
 *  i         | stride      stride      register 
 *  j         | cont        register    cont 
 *  k         | register    cont        cont  
 *=======================================================================*/
void mmp_lex_tp_blk(const int ns, const int nbk1, const int nbk2, 
  const int nbkr, const int nbkc, 
  double mc[restrict][ns], double ma[restrict][ns], double mb[restrict][ns])
/* use of variable arrays in C99 
 * note: double A[][ns] is equal to double (*A)[ns]  */
{
  int i, j, k, jj, kk;
  double work[ns][ns];

#if ! defined(NOT_USE_TRANS_BLK)
  trans_blk(ns,ns,nbkr,nbkc,work,mb); 
#else
  trans(ns,ns,work,mb); 
#endif

  for ( i = 0; i < ns; ++i ) {
  for ( j = 0; j < ns; ++j ) {
     mc[i][j] = 0.0;  
  }}

  /* (cache size/core)/8.0bytes > nbk1*nbk2 + nbk1                     */
  for ( jj = 0; jj < ns; jj += nbk2 ) {
  for ( kk = 0; kk < ns; kk += nbk1 ) {
     for ( i = 0; i < ns; ++i ) {
     for ( j= jj; j < MIN(ns,jj+nbk2); ++j ) {
        for ( k= kk; k < MIN(ns,kk+nbk1); ++k ) {
           mc[i][j] += ma[i][k]*work[j][k];
        }
     }}
  }}
}

/*=========================================================================
 *  Transpose routine
 *=======================================================================*/
void trans(const int nraw, const int ncol, double tm[restrict][nraw], 
  double m[restrict][ncol])
/* use of variable arrays in C99 
 * note: double A[][ns] is equal to double (*A)[ns]  */
{
  int i, j;

  for ( i = 0; i < ncol; ++i ) {
  for ( j = 0; j < nraw; ++j ) {
      tm[i][j] = m[j][i];
  }}
}

/*=========================================================================
 *  Transpose routine with blocking
 *=======================================================================*/
void trans_blk(const int nraw, const int ncol, const int nbkr, const int nbkc, 
  double tm[restrict][nraw], double m[restrict][ncol])
/* use of variable arrays in C99 
 * note: double A[][ns] is equal to double (*A)[ns]  */
{
  int i, j, ii, jj;

  for ( ii = 0; ii < ncol; ii += nbkc) {
  for ( jj = 0; jj < nraw; jj += nbkr) {
     for ( i = ii; i < MIN(ncol,ii+nbkc); ++i ) {
     for ( j = jj; j < MIN(nraw,jj+nbkr); ++j ) {
        tm[i][j] = m[j][i];
     }}
  }}
}

/*=========================================================================
 *  Dummy routine
 *=======================================================================*/
int dummy(const int ns, double mat[restrict][ns])
/* use of variable arrays in C99 
 * note: double A[][ns] is equal to double (*A)[ns]  */
{
  int u = 0;
  return u;
}

/* Copyright 2025 Research Organization for Information Science and Technology */
#include "mykernel.hpp"
/*=========================================================================
 * Simple implementation
 * mc: NNxMM, ma: NNxLL, mb: LLxMM
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
void mmp_simple (const int NN, const int MM, const int LL, 
  double * noalias mc, double * noalias ma, double * noalias mb)
{
  int i, j, k, ji;

  for ( ji = 0; ji < NN*MM; ++ji ) {
     mc[ji] = 0.0; 
  }

  for ( i = 0; i < NN; ++i ) {
  for ( k = 0; k < LL; ++k ) {
      int ki = k + LL*i;
  for ( j = 0; j < MM; ++j ) {
      int ji = j + MM*i;
      int jk = j + MM*k;
      mc[ji] += ma[ki]*mb[jk]; /* mc[i][j] += ma[i][k]*mb[k][j]; */
  }}}
}

/*=========================================================================
 * Simple implementation with OpenACC
 * mc: NNxMM, ma: NNxLL, mb: LLxMM
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
void mmp_simple_acc (const int NN, const int MM, const int LL, 
  double * noalias mc, double * noalias ma, double * noalias mb)
{
  #pragma acc parallel present(ma[0:NN*LL],mb[0:LL*MM],mc[0:NN*MM])
  {

  #pragma acc loop independent 
  for ( int ji = 0; ji < NN*MM; ++ji ) {
     mc[ji] = 0.0; 
  }
 
  #pragma acc loop collapse(2) independent
  for ( int i = 0; i < NN; ++i ) {
  for ( int j = 0; j < MM; ++j ) {
     double mctmp = 0.0;
     #pragma acc loop seq
     for ( int k = 0; k < LL; ++k ) {
        mctmp += ma[k + LL*i]*mb[j + MM*k]; /* mctmp += ma[i][k]*mb[k][j] */
     }
     mc[j + MM*i] += mctmp; /* mc[i][j] */
  }}

  } /* acc parallel */
}

/*=========================================================================
 * Simple implementation with OpenACC: gang-vector 
 * mc: NNxMM, ma: NNxLL, mb: LLxMM
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
void mmp_simple_acc_gv (const int NN, const int MM, const int LL, 
  double * noalias mc, double * noalias ma, double * noalias mb)
{
  #pragma acc parallel present(ma[0:NN*LL],mb[0:LL*MM],mc[0:NN*MM])
  {

  #pragma acc loop independent 
  for ( int ji = 0; ji < NN*MM; ++ji ) {
     mc[ji] = 0.0; 
  }
 
  #pragma acc loop gang independent
  for ( int i = 0; i < NN; ++i ) {
  #pragma acc loop vector independent
  for ( int j = 0; j < MM; ++j ) {
     double mctmp = 0.0;
     #pragma acc loop seq
     for ( int k = 0; k < LL; ++k ) {
        mctmp += ma[k + LL*i]*mb[j + MM*k]; /* mctmp += ma[i][k]*mb[k][j] */
     }
     mc[j + MM*i] += mctmp; /* mc[i][j] */
  }}

  } /* acc parallel */
}

/*=========================================================================
 * dummy
 *=======================================================================*/
int dummy (const int it, const int n, double * noalias mat) 
{
   int icon = 0;
   return icon;
}

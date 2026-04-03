/* Copyright 2025 Research Organization for Information Science and Technology */
#include <stdio.h>
#include <stdlib.h>

#define NSIZE 1024
#define IND(I,J) ( (J) + (I) * (NSIZE) )

int main(int argc, char **argv)
{
  float *A, *B, *C;
  float *D, *E, *F;

  A = (float *)malloc( sizeof(float)*NSIZE ); 
  B = (float *)malloc( sizeof(float)*NSIZE ); 
  C = (float *)malloc( sizeof(float)*NSIZE ); 

  D = (float *)malloc( sizeof(float)*NSIZE*NSIZE ); 
  E = (float *)malloc( sizeof(float)*NSIZE*NSIZE ); 
  F = (float *)malloc( sizeof(float)*NSIZE*NSIZE ); 

  for (int i = 0; i < NSIZE; ++i ) {
    A[i] = 1.0f;
    B[i] = 2.0f;
    C[i] = 3.0f;
  }

  for (int i = 0; i < NSIZE; ++i ) {
  for (int j = 0; j < NSIZE; ++j ) {
     D[IND(i,j)] = 1.0f;
     E[IND(i,j)] = 2.0f;
     F[IND(i,j)] = 3.0f;
  }}
#if 0
  /* ======================================================================
   * parallel with no loop  
   * ====================================================================*/
  #pragma acc parallel
  {
  A[0] = B[0] + c[0]; /* meaningless code ... */
  } /* acc parallel */
#endif
  /* ======================================================================
   * kernels with two loops  
   * ====================================================================*/
  #pragma acc kernels
  {
  for (int i = 0; i < NSIZE; ++i ) {
     A[i] = B[i] + C[i];
  }

  for (int i = 0; i < NSIZE; ++i ) {
  for (int j = 0; j < NSIZE; ++j ) {
     D[IND(i,j)] = E[IND(i,j)]*F[IND(i,j)];
  }}
  } /* acc kernels */
  /* ======================================================================
   * parallel with two loops  
   * ====================================================================*/
  #pragma acc parallel
  {
  for (int i = 0; i < NSIZE; ++i ) {
     A[i] = B[i] + C[i];
  }

  for (int i = 0; i < NSIZE; ++i ) {
  for (int j = 0; j < NSIZE; ++j ) {
     D[IND(i,j)] = E[IND(i,j)]*F[IND(i,j)];
  }}
  } /* acc parallel */
  /* ======================================================================
   * parallel with two loops: Secifying loop directives  
   * ====================================================================*/
  #pragma acc parallel
  {
  #pragma acc loop
  for (int i = 0; i < NSIZE; ++i ) {
     A[i] = B[i] + C[i];
  }

  #pragma acc loop
  for (int i = 0; i < NSIZE; ++i ) {
  for (int j = 0; j < NSIZE; ++j ) {
     D[IND(i,j)] = E[IND(i,j)]*F[IND(i,j)];
  }}
  } /* acc parallel */
  /* ======================================================================
   * parallel with two loops: Specifying loop directives with independent 
   * ====================================================================*/
  #pragma acc parallel
  {
  #pragma acc loop
  for (int i = 0; i < NSIZE; ++i ) {
     A[i] = B[i] + C[i];
  }

  #pragma acc loop independent
  for (int i = 0; i < NSIZE; ++i ) {
  #pragma acc loop independent
  for (int j = 0; j < NSIZE; ++j ) {
     D[IND(i,j)] = E[IND(i,j)]*F[IND(i,j)];
  }}
  } /* acc parallel */

  /* ======================================================================
   * parallel with two loops: Specifying loop directives with collapse 
   * ====================================================================*/
  #pragma acc parallel
  {
  #pragma acc loop
  for (int i = 0; i < NSIZE; ++i ) {
     A[i] = B[i] + C[i];
  }

  #pragma acc loop collapse(2) independent
  for (int i = 0; i < NSIZE; ++i ) {
  for (int j = 0; j < NSIZE; ++j ) {
     D[IND(i,j)] = E[IND(i,j)]*F[IND(i,j)];
  }}
  } /* acc parallel */

  return EXIT_SUCCESS;
}

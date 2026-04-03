/* Copyright 2024 Research Organization for Information Science and Technology */
#include <stdio.h>
#include <stdlib.h>

#define NSIZE 8

int main (int argc, char ** argv)
{
  int a[NSIZE], f[NSIZE][NSIZE];

  /* [Note] collapse clause
     collapse(2) means the 2 successive loops can be converted
     into 1 single loop suitable for parallel calculations. */
  #pragma omp parallel for schedule(static) collapse(2)
  for ( int i = 0; i< NSIZE; ++i ) {
  for ( int j = 0; j< NSIZE; ++j ) {
     f[i][j] = i - j;
  }}

  #pragma omp parallel for schedule(static)
  for (int i=0; i<NSIZE; ++i ) {
     a[i] = 0;
  }

  /* For OpenMP version 5.0 
  #pragma omp parallel for schedule(static) reduction(+:a)
  */
  /* For OpenMP version 4.5 */
  int ns = NSIZE;
  #pragma omp parallel for schedule(static) reduction(+:a[0:ns])
  for ( int i=0; i<NSIZE; ++i ) {
    for (int j=0; j<NSIZE; ++j ) {
       a[i] += f[i][j]; 
    }
  }

  for ( int i = 0; i<NSIZE; ++i ) {
    printf("a[%d]=%d\n",i,a[i]);
  }

  return EXIT_SUCCESS;
}

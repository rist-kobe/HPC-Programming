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

  /* Each thread gets a private copy of the whole array a, initialized
     to 0; OpenMP combines the private copies element-wise into the
     original array at the end of the loop. The parallel loop runs over
     the second index j, so every thread accumulates into every a[i]
     and the array reduction is essential.                             */
  /* The whole-array form reduction(+:a) requires OpenMP 5.0.
     For OpenMP 4.5, use the array-section form instead:
     #pragma omp parallel for schedule(static) reduction(+:a[0:NSIZE])
  */
  #pragma omp parallel for schedule(static) reduction(+:a)
  for ( int j=0; j<NSIZE; ++j ) {
    for (int i=0; i<NSIZE; ++i ) {
       a[i] += f[i][j]; 
    }
  }

  for ( int i = 0; i<NSIZE; ++i ) {
    printf("a[%d]=%d\n",i,a[i]);
  }

  return EXIT_SUCCESS;
}

/* Copyright 2025 Research Organization for Information Science and Technology */
/* See
 * Reductions on pointer type must have bounds specified 
 * https://forums.developer.nvidia.com/t/reductions-on-pointer-type-must-have-bounds-specified/227878
 */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NSIZE 8

int main (int argc, char ** argv)
{
  {
  int ng = omp_get_num_devices();
  if ( ng == 0 ) {
     fprintf(stdout,"There is no available device.\n");
     return EXIT_SUCCESS;
  } else {
     fprintf(stdout,"%d OpenMP Target Capable devices are detected.\n", ng);
  }
  }

#if 0
  int *a, *f;
  a = (int *)malloc( sizeof(int)*NSIZE );
#else
  int *f;
  int a[NSIZE];
  f = (int *)malloc ( sizeof(int)*NSIZE*NSIZE );
#endif

  #pragma omp target data map(alloc:f[0:NSIZE*NSIZE]) map(from:a[0:NSIZE]) 
  {
  #pragma omp target map(present:f)
  #pragma omp loop collapse(2)
  for ( int i = 0; i< NSIZE; ++i ) {
  for ( int j = 0; j< NSIZE; ++j ) {
     int ij = j + i*NSIZE;
     f[ij] = i - j;
  }}

  #pragma omp target map(present:a)
  #pragma omp loop 
  for (int i=0; i<NSIZE; ++i ) {
     a[i] = 0;
  }

  #pragma omp target map(present:a,f)
  #pragma omp loop reduction(+:a[0:NSIZE])
  for ( int i=0; i<NSIZE; ++i ) {
    for (int j=0; j<NSIZE; ++j ) {
       int ij = j + i*NSIZE;
       a[i] += f[ij]; 
    }
  }

  } /* omp target data */

  for ( int i = 0; i<NSIZE; ++i ) {
    printf("a[%d]=%d\n",i,a[i]);
  }

  return EXIT_SUCCESS;
}

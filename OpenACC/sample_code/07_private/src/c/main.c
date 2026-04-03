/* Copyright 2024 Research Organization for Information Science and Technology */
#include "mykernel.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define NSIZE 1024
#define NREP  10000

int main (int argc, char ** argv)
{
  int s;
  double elp, elp0;
  float tmp;
  float *a;

  a = (float *)malloc( sizeof(float)*NSIZE );

  /* warm up */
  for (int i=0; i<NSIZE; ++i) {
    a[i] = 1.0f;
  }

  func_3(NSIZE,a); 

  /* func_0 */
  for (int i=0; i<NSIZE; ++i) {
    a[i] = 1.0f;
  }

  elp0 = omp_get_wtime();

  s = 0;

  for ( int i=0; i<NREP; ++i ) {
     func_0(NSIZE,a);
     s = dummy(NSIZE,a);
     s = s + i;
     for (int i=0; i<NSIZE; ++i) {
       a[i] = 1.0f;
     }
  }

  elp = omp_get_wtime() - elp0;

  tmp = 0.0f;
  for ( int i = 0; i<NSIZE; ++i) 
    tmp += a[i];

  printf("[func_0] elpase_sec: %10.3f check: %15.8E\n",elp,tmp);

  /* func_1 */
  for (int i=0; i<NSIZE; ++i) {
    a[i] = 1.0f;
  }

  elp0 = omp_get_wtime();

  s = 0;

  for ( int i=0; i<NREP; ++i ) {
     func_1(NSIZE,a);
     s = dummy(NSIZE,a);
     s = s + i;
     for (int i=0; i<NSIZE; ++i) {
       a[i] = 1.0f;
     }
  }

  elp = omp_get_wtime() - elp0;

  tmp = 0.0f;
  for ( int i = 0; i<NSIZE; ++i) 
    tmp += a[i];

  printf("[func_1] elpase_sec: %10.3f check: %15.8E\n",elp,tmp);

  /* func_2 */
  for (int i=0; i<NSIZE; ++i) {
    a[i] = 1.0f;
  }

  elp0 = omp_get_wtime();

  s = 0;

  for ( int i=0; i<NREP; ++i ) {
     func_2(NSIZE,a);
     s = dummy(NSIZE,a);
     s = s + i;
     for (int i=0; i<NSIZE; ++i) {
       a[i] = 1.0f;
     }
  }

  elp = omp_get_wtime() - elp0;

  tmp = 0.0f;
  for ( int i = 0; i<NSIZE; ++i) 
    tmp += a[i];

  printf("[func_2] elpase_sec: %10.3f check: %15.8E\n",elp,tmp);

  /* func_3 */
  for (int i=0; i<NSIZE; ++i) {
    a[i] = 1.0f;
  }

  elp0 = omp_get_wtime();

  s = 0;

  for ( int i=0; i<NREP; ++i ) {
     func_3(NSIZE,a);
     s = dummy(NSIZE,a);
     s = s + i;
     for (int i=0; i<NSIZE; ++i) {
       a[i] = 1.0f;
     }
  }

  elp = omp_get_wtime() - elp0;

  tmp = 0.0f;
  for ( int i = 0; i<NSIZE; ++i) 
    tmp += a[i];

  printf("[func_3] elpase_sec: %10.3f check: %15.8E\n",elp,tmp);

  free( a );

  return EXIT_SUCCESS;
}

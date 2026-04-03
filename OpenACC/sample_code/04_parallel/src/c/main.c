/* Copyright 2024 Research Organization for Information Science and Technology */
#include "mykernel.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define NSIZE 80000
#define NREP  10000

int main (int argc, char ** argv)
{
  int s;
  double elp, elp0;
  float tmp;
  float *a, *b, *c;

  a = (float *)malloc( sizeof(float)*NSIZE );
  b = (float *)malloc( sizeof(float)*NSIZE );
  c = (float *)malloc( sizeof(float)*NSIZE );

  /* add */
  for (int i=0; i<NSIZE; ++i) {
    a[i] = 0.0;
    b[i] = 1.0;
    c[i] = 2.0;
  }

  elp0 = omp_get_wtime();

  s = 0;

#if ! defined(USE_UNIFIED_MEM)
#pragma acc data copyin(b[0:NSIZE],c[0:NSIZE]) copy(a[0:NSIZE])
{
#endif
  for ( int i=0; i<NREP; ++i ) {
     add(NSIZE,a,b,c);
     s = dummy(NSIZE,a);
     s = s + i;
  }
#if ! defined(USE_UNIFIED_MEM)
}
#endif

  elp = omp_get_wtime() - elp0;

  printf("[ADD] elpase_sec: %10.3f\n",elp);

  /* saxpy */
  for (int i=0; i<NSIZE; ++i) {
    a[i] = 0.0;
    b[i] = 1.0;
  }

  tmp = 1.0f;

  elp0 = omp_get_wtime();

  s = 0;

#if ! defined(USE_UNIFIED_MEM)
#pragma acc data copyin(b[0:NSIZE]) copy(a[0:NSIZE])
{
#endif
  for ( int i=0; i<NREP; ++i ) {
     saxpy(NSIZE,tmp,b,a);
     s = dummy(NSIZE,a);
     s = s + i;
  }
#if ! defined(USE_UNIFIED_MEM)
}
#endif

  elp = omp_get_wtime() - elp0;

  printf("[SAXPY] elpase_sec: %10.3f\n",elp);

  free( a );
  free( b );
  free( c );

  return 0;
}

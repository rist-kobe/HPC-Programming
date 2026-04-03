/* Copyright 2025 Research Organization for Information Science and Technology */
#include "mykernel.h"
#include <stdio.h>
#include <stdlib.h>

#if defined(_OPENMP)
#include <omp.h>
#endif
#if defined(_OPENACC)
#include <openacc.h>
#endif

#define NSIZE 10000
#define NREP  10000

#define PRINTAPI(x) printf("%s=%d\n", #x, x)

int main (int argc, char ** argv)
{
  int s;
  double elp, elp0;
  float tmp;
  float *x, *y;

  {
#if defined(_OPENMP)
  PRINTAPI(_OPENMP);
  int nt = omp_get_max_threads();
  fprintf(stdout,"Number of OpenMP host threads     : %d\n", nt);
#endif
#if defined(_OPENACC)
  PRINTAPI(_OPENACC);
  acc_device_t dev_type = acc_get_device_type();
  int ng = acc_get_num_devices(dev_type);
  if ( ng == 0 ) {
     fprintf(stdout,"There is no available device\n");
     return EXIT_FAILURE; 
  } 
  fprintf(stdout,"Number of OpenACC capable devices : %d\n", ng);
#endif
  }

  x = (float *)malloc( sizeof(float)*NSIZE );
  y = (float *)malloc( sizeof(float)*NSIZE );


  /* saxpy0 */
  #pragma omp parallel for schedule(static)
  for (int i=0; i<NSIZE; ++i) {
    x[i] = 2.0f;
    y[i] = 1.0f;
  }

  saxpy_0(NSIZE,3.0f,x,y); /* warm up */

  elp0 = omp_get_wtime();

  s = 0;

  for ( int i=0; i<NREP; ++i ) {
     saxpy_0(NSIZE,3.0f,x,y);
     s = dummy(NSIZE,x);
     s = s + i;
  }

  elp = omp_get_wtime() - elp0;

  tmp = 0.0f;
  for ( int i = 0; i<NSIZE; ++i) 
    tmp += y[i];

  printf("[SAXPY_0] elpase_sec: %10.3f check: %15.8E\n",elp,tmp);

  /* saxpy1 */
  #pragma omp parallel for schedule(static)
  for (int i=0; i<NSIZE; ++i) {
    x[i] = 2.0f;
    y[i] = 1.0f;
  }

  saxpy_1(NSIZE,3.0f,x,y); /* warm up */

  elp0 = omp_get_wtime();

  s = 0;

  for ( int i=0; i<NREP; ++i ) {
     saxpy_1(NSIZE,3.0f,x,y);
     s = dummy(NSIZE,x);
     s = s + i;
  }

  elp = omp_get_wtime() - elp0;

  tmp = 0.0f;
  for ( int i = 0; i<NSIZE; ++i) 
    tmp += y[i];

  printf("[SAXPY_1] elpase_sec: %10.3f check: %15.8E\n",elp,tmp);

  /* saxpy_omp_0 */
  #pragma omp parallel for schedule(static)
  for (int i=0; i<NSIZE; ++i) {
    x[i] = 2.0f;
    y[i] = 1.0f;
  }

  saxpy_omp_0(NSIZE,3.0f,x,y);

  elp0 = omp_get_wtime();

  s = 0;

  for ( int i=0; i<NREP; ++i ) {
     saxpy_omp_0(NSIZE,3.0f,x,y);
     s = dummy(NSIZE,x);
     s = s + i;
  }

  elp = omp_get_wtime() - elp0;

  tmp = 0.0f;
  for ( int i = 0; i<NSIZE; ++i) 
    tmp += y[i];

  printf("[SAXPY_OMP_0] elpase_sec: %10.3f check: %15.8E\n",elp,tmp);

  free( x );
  free( y );

  return 0;
}

/* Copyright 2024 Research Organization for Information Science and Technology */
#include <stdio.h>
#include <stdlib.h>

#include "mykernel.h"
#include "timer.h"

#define NSIZE 1024
#define NREP  10000

typedef void (*saxpy_fn)(const int ns, float a, float * restrict x,
                         float * restrict y);

/* Expected checksum for one pass: NSIZE * a * 0.0001 = 0.3072 */
static void run_benchmark(const char * label, saxpy_fn kernel,
                          const int use_data_region, float * x, float * y)
{
  int s;
  double elp, elp0;
  float tmp;

  (void)use_data_region; /* only used by the offloading data directives */

  for (int i=0; i<NSIZE; ++i) {
    x[i] = 0.0001f;
  }
  for (int i=0; i<NSIZE; ++i) {
    y[i] = 0.0f;
  }

  kernel(NSIZE,3.0f,x,y); /* warm up */

  elp0 = get_elp_time();

  s = 0;

#if _OPENACC
  #pragma acc data copyin(x[0:NSIZE]) if(use_data_region)
#endif
#if _OPENMP
  #pragma omp target data map(to:x[0:NSIZE]) if(use_data_region)
#endif
  for ( int irep=0; irep<NREP; ++irep ) {
     for (int i=0; i<NSIZE; ++i) {
       y[i] = 0.0f;
     }
     kernel(NSIZE,3.0f,x,y);
     s += dummy(NSIZE,x);
  }

  elp = get_elp_time() - elp0;

  tmp = 0.0f;
  for ( int i = 0; i<NSIZE; ++i) 
    tmp += y[i];

  printf("[%s] elapsed_sec: %10.3f check: %15.8E s: %d\n",label,elp,tmp,s);
}

int main (int argc, char ** argv)
{
  float *x, *y;

  x = (float *)malloc( sizeof(float)*NSIZE );
  y = (float *)malloc( sizeof(float)*NSIZE );

  run_benchmark("SAXPY_0", saxpy_0, 0, x, y);
  run_benchmark("SAXPY_1", saxpy_1, 0, x, y);
  run_benchmark("SAXPY_2", saxpy_2, 1, x, y);

  free( x );
  free( y );

  return 0;
}

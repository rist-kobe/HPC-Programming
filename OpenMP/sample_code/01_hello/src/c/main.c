/* Copyright 2024 Research Organization for Information Science and Technology */
#if defined(_OPENMP)
#include <omp.h>
#endif
#include <stdio.h>

int main (int argc, char **argv) 
{
  int nthreads;
  int tid;

#if defined(_OPENMP)
  nthreads = omp_get_max_threads();
#else
  nthreads=1;
  tid = 0;
#endif

  printf("Hello.\n");
  #pragma omp parallel private(tid)
  {
#if defined(_OPENMP)
    tid = omp_get_thread_num();
#endif
/* Use of omp critical is corrent. Why? */
    #pragma omp critical 
      printf("Hello. I am thread-ID %3d of %3d threads\n", tid, nthreads);
/* Naive implementation.
      printf("Hello. I am thread-ID %3d of %3d threads\n", tid, nthreads);
*/
  }
  return 0;
}

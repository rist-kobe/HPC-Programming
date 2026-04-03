/* Copyright 2024 Research Organization for Information Science and Technology */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define PRINTAPI(x) printf("%s=%d\n", #x, x)

int main (int argc, char **argv)
{
  PRINTAPI(_OPENMP);
  #pragma omp parallel
  {
    int nt = omp_get_num_threads();
    int tid = omp_get_thread_num();
    if ( tid == 0 ) {
       fprintf(stdout,"I am the primary (0) thread. Total number of threads = %d\n", nt);
    }
  }
  return EXIT_SUCCESS;
}

// Copyright 2024 Research Organization for Information Science and Technology 
#include <cstdio>
#include <cstdlib>
#include <omp.h>
#include "myclass.h"

#define NSIZE 50

int main (int argc, char **argv)
{
  int i, ifrom, ito; 
  int s;
  int a[NSIZE],b[NSIZE];
  MyClass worker;

  #pragma omp parallel for schedule(static) private(i) shared(a,b)
  for (i = 0; i < NSIZE; ++i ) {
    a[i] = 0;
    b[i] = 0;
  }

  b[0] = 3;
  b[1] = 0;
  b[2] = 2;
  for (i = 3; i < NSIZE; ++i ) {
    b[i] = b[i-2] + b[i-3];
  }

  #pragma omp parallel shared(s,a,b) private(i,ifrom,ito) 
  {
     int tid = omp_get_thread_num ();
     int nt  = omp_get_num_threads ();

     worker.decomp_size(NSIZE, tid, nt, ifrom, ito); 

#if 0
     #pragma omp critical (CHECKDECOMP)
     {
        fprintf(stdout,"tid=%d ifron=%d ito=%d\n", tid, ifrom, ito);
     }
#endif

     #pragma omp for schedule(static)
     for (i=1; i < NSIZE - 1; ++i ) {
        a[i] = (b[i-1] + b[i+1])/NSIZE;
     }

     #pragma omp single
     { s = 0; }

#if ! defined(WO_CRITICAL)
     #pragma omp critical (FUNC)
     { s += worker.func(a,ifrom,ito); }
#else
     // wrong implementation: Please consider the reason
     s += worker.func(a,ifrom,ito); 
#endif
  } // omp parallel

  fprintf(stdout,"%d\n",s); 
  return EXIT_SUCCESS;
}

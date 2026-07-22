// Copyright 2024 Research Organization for Information Science and Technology 
#include <cstdio>
#include <cstdlib>
#include <omp.h>
#include "myclass.h"

#define NSIZE 50

int main (int argc, char **argv)
{
  int s;
  int a[NSIZE],b[NSIZE];
  MyClass worker;

  #pragma omp parallel for schedule(static) shared(a,b)
  for (int i = 0; i < NSIZE; ++i ) {
    a[i] = 0;
    b[i] = 0;
  }

  b[0] = 3;
  b[1] = 0;
  b[2] = 2;
  for (int i = 3; i < NSIZE; ++i ) {
    b[i] = b[i-2] + b[i-3];
  }

  #pragma omp parallel shared(s,a,b) 
  {
     int ifrom, ito;
     int tid = omp_get_thread_num ();
     int nt  = omp_get_num_threads ();

     worker.decomp_size(NSIZE, tid, nt, ifrom, ito); 

#if 0
     #pragma omp critical (CHECKDECOMP)
     {
        fprintf(stdout,"tid=%d ifrom=%d ito=%d\n", tid, ifrom, ito);
     }
#endif

     #pragma omp for schedule(static)
     for (int i=1; i < NSIZE - 1; ++i ) {
        a[i] = (b[i-1] + b[i+1])/NSIZE;
     }

     #pragma omp single
     { s = 0; }

#if defined(BETTER_IMPL)
     // better implementation: func_local keeps its work array on the
     // stack (no shared state), so only the update of the shared
     // variable s must be protected, and atomic is enough
     int ds = worker.func_local(a,ifrom,ito);
     #pragma omp atomic update
     s += ds;
#elif ! defined(WO_CRITICAL)
     // The member array MyClass::u is shared among the threads, so the
     // whole call to func must be protected by a critical construct.
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

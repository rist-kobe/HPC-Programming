/* Copyright 2024 Research Organization for Information Science and Technology */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define NSIZE 4

int func_a (int, int );
int func_b (const int, int, int, int* );

int main(int argc, char **argv)
{
  int tid, nthreads;
  int a[NSIZE], b[NSIZE];

  nthreads = omp_get_max_threads ();
  if ( nthreads != NSIZE ) {
    printf("[Error] Number of threads must be equal to 4\n");
    return EXIT_FAILURE;
  }

  /* w/ omp_barrier */
  for ( tid = 0; tid < NSIZE; ++tid ) {
    a[tid] = b[tid] = 0;
  }

  /* [Note] num_threads clasue
     num_threads(4) means the number of threads is forced 
     to be 4 in the corresponding parallel region.       */
  #pragma omp parallel private(tid,nthreads) \
  num_threads(4)
  {
     tid = omp_get_thread_num ();
     nthreads = omp_get_num_threads ();
     a[tid] = func_a (tid, nthreads);
     #pragma omp barrier
     b[tid] = func_b (NSIZE, tid, nthreads, a);
  }

  printf("[With omp_barrier]\n");
  for ( int i = 0; i<NSIZE; ++i) 
    printf("a[%d] = %4d, b[%d] = %4d\n",i,a[i],i,b[i]);

  /* w/o omp_barrier */
  for ( tid = 0; tid < NSIZE; ++tid ) {
    a[tid] = b[tid] = 0;
  }

  #pragma omp parallel private(tid,nthreads) \
  num_threads(4) 
  {
     tid = omp_get_thread_num ();
     nthreads = omp_get_num_threads ();
     a[tid] = func_a (tid, nthreads);
     /* func_b requires full update of a */
     b[tid] = func_b (NSIZE, tid, nthreads, a); 
  }

  printf("[Without omp_barrier]\n");
  for ( int i = 0; i<NSIZE; ++i) 
    printf("a[%d] = %4d, b[%d] = %4d\n",i,a[i],i,b[i]);

  return EXIT_SUCCESS;
}

int func_a (int tid, int nt)
{
   int s;
   return s = (nt+tid*(nt + tid*(nt + tid*(nt+tid))));
}

int func_b (const int ns, int tid, int nt, int *a)
{
   int s,ind;
   ind = (tid + 2 >= ns ) ? (tid + 2 - ns) : (tid + 2);
   return s = tid*nt + a[ind];
}

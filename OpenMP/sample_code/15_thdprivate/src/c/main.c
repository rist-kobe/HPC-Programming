/* Copyright 2024 Research Organization for Information Science and Technology */
/*-----------------------------------------------------
! Counter of a negative value
! The original idea come from Fig 10.7 in Sec.10.2
! in T. G. Mattson, Y. (Helen) He, and A. E. Kniges,
! The OpenMP Common core: Making OpenMP Simple Again
! (MIT,2019)
! http://www.ompcore.com
!-----------------------------------------------------*/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int count;
#pragma omp threadprivate(count)

void inc ()
{
  count++;
}

int func ( int x ) 
{
  return x;
}

int main (int argc, char** argv)
{
  int nsize; 
  int v;
  int *a;

  if ( argc != 2 ) {
    fprintf(stderr, "[usage] run.x (arg1)\n  arg1: array size (int)\n");
    return EXIT_FAILURE;
  }
  nsize = atoi (argv[1]);
  if ( nsize <= 0 ) {
     fprintf(stderr, "Error: array size must be a positive integer.\n");
     return EXIT_FAILURE;
  }

  a = (int *)malloc ( nsize*sizeof(int) );

  for (int i=0; i<nsize; ++i) {
     a[i] = i - nsize/2;
  }

  /* Disable dynamic thread adjustment: the threadprivate values
     persist between the two parallel regions only when dynamic
     adjustment is off and the number of threads is unchanged.    */
  omp_set_dynamic(0);

  count = 0;
  /* 1st round */
  #pragma omp parallel private(v) copyin(count) 
  {
    int tid = omp_get_thread_num ();
    #pragma omp for schedule(static)
    for (int i=0; i<nsize; ++i ) {
       v = func ( a[i] ) ;
       if ( v < 0 ) { inc (); }
    }    
    #pragma omp critical
    { printf("Round 1: thread ID=%d,  # of negative v: %4d\n",tid,count); }
  }

  /* dump count on thread ID=0 */
  printf("b/w Rounds 1 and 2: primary thread, # of negative v: %4d\n",count);

  /* 2nd round: copyin is deliberately omitted, so each thread's
     count retains its value from Round 1 and keeps accumulating. */
  #pragma omp parallel private(v)
  {
    int tid = omp_get_thread_num ();
    #pragma omp for schedule(static)
    for (int i=0; i<nsize; ++i ) {
       v = func ( a[i] ) ;
       if ( v < 0 ) { inc (); }
    }
    #pragma omp critical
    { printf("Round 2: thread ID=%d,  # of negative v: %4d\n",tid,count); }
  }

  /* Reference: number of negatives per round for a self-check     */
  printf("Expected # of negative v per round: %4d\n", nsize/2);

  free(a);
  return EXIT_SUCCESS;
}

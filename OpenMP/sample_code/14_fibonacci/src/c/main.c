/* Copyright 2024 Research Organization for Information Science and Technology */
/*----------------------------------------------------
  Fibonacci series
  The original code is from Fig.7.11 in 
  T. G. Mattson, Y. (Helen) He, and A. E. Kniges,
  The OpenMP Common core: Making OpenMP Simple Again
  (MIT,2019)
  http://www.ompcore.com
----------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int fib_omp ( int n )
{
  int x,y;
  if ( n < 2 ) return n;
  #pragma omp task shared(x) 
  { x = fib_omp (n-1);}
  #pragma omp task shared(y)
  { y = fib_omp (n-2);}
  #pragma omp taskwait
  return (x+y);
}

int main (int argc, char** argv)
{
  for ( int i=1; i<=10; ++i ) {
    int vn;
    #pragma omp parallel 
    {
    #pragma omp single
    { vn=fib_omp (i); }
    }
    printf("fibonacci_%d = %3d\n",i,vn);
  }
  return EXIT_SUCCESS;
}

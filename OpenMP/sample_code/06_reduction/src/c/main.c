/* Copyright 2024 Research Organization for Information Science and Technology */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define N 100

int main(void)
{
   int a[N];
   int amax;

   for ( int i = 0; i < N; ++i ) {
       a[i] = 0;
   }
   a[0] = 1;

   /* INT_MIN is the identity value of a max reduction: it is correct
    * even when every element of the array is negative. OpenMP itself
    * initializes each thread's private copy of amax with the minimum
    * representable value and combines the private results at the end
    * of the loop.                                                    */
   amax = INT_MIN;
   #pragma omp parallel for reduction(max:amax)
   for (int i = 0; i < N; ++i ) {
       if ( a[i] > amax ) {
          amax = a[i];
       }
   }
   printf("Max val of array a: %d\n", amax);
   return EXIT_SUCCESS;
}

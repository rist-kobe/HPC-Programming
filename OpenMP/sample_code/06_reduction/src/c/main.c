/* Copyright 2024 Research Organization for Information Science and Technology */
#include <stdio.h>
#include <stdlib.h>

#define MAX(a,b) ( (a) > (b) ? (a) : (b) ) 

int main(void)
{
   int a[100];
   int amax;

   for ( int i = 0; i< 100; ++i ) {
       a[i] = 0;
   }
   a[0] = 1;

   amax=0;
   #pragma omp parallel for reduction(max:amax)
   for (int i = 0; i< 100; ++i ) {
       amax = MAX(amax,a[i]);
   }
   printf("Max val of array a: %d\n", amax);
   return EXIT_SUCCESS;
}

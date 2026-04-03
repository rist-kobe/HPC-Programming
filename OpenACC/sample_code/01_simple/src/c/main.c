/* Copyright 2025 Research Organization for Information Science and Technology */
#include <stdio.h>
#include <stdlib.h>

#define N 64

int main (int argc, char **argv)
{
   float *a, *b;

   a = (float *)malloc ( N*sizeof(float) ); 
   b = (float *)malloc ( N*sizeof(float) ); 

   for ( int i = 0; i < N; ++i ) {
      a[i] = i*1.0f;
      b[i] = i*10.0f;
   }

   #pragma acc kernels loop independent
   for ( int i = 0; i < N; ++i ) {
      a[i] = a[i] + 2.0f;
      b[i] = b[i] * 2.0f;
   }

   for ( int i = 0; i < N; ++i ) {
      printf("%f %f\n", a[i], b[i]);
   }

   return EXIT_SUCCESS;
}

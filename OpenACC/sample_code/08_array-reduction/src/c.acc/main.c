/* Copyright 2024 Research Organization for Information Science and Technology */
#include <stdio.h>
#include <stdlib.h>
#include <openacc.h>

#define NSIZE 8

int main (int argc, char ** argv)
{
  {
  acc_device_t dev_type = acc_get_device_type();
  int ng = acc_get_num_devices(dev_type);
  if ( ng == 0 ) {
      fprintf(stdout,"There is no available device.\n");
      return EXIT_SUCCESS;
  } else {
     fprintf(stdout,"%d OpenACC Capable devices are detected.\n", ng);
  }
  }

  int *a, *f;

  a = (int *)malloc( sizeof(int)*NSIZE );
  f = (int *)malloc( sizeof(int)*NSIZE*NSIZE );

  #pragma acc data create(f[0:NSIZE*NSIZE]) copyout(a[0:NSIZE])
  {

  #pragma acc kernels 
  #pragma acc loop independent collapse(2)
  for ( int i = 0; i< NSIZE; ++i ) {
  for ( int j = 0; j< NSIZE; ++j ) {
     int ij = j + i*NSIZE;
     f[ij] = i - j;
  }}

  #pragma acc kernels
  #pragma acc loop independent
  for (int i=0; i<NSIZE; ++i ) {
     a[i] = 0;
  }

  #pragma acc parallel reduction(+:a[0:NSIZE]) 
  #pragma acc loop 
  for ( int i=0; i<NSIZE; ++i ) {
    for (int j=0; j<NSIZE; ++j ) {
       int ij = j + i*NSIZE;
       a[i] += f[ij]; 
    }
  }

  } /* acc data */

  for ( int i = 0; i<NSIZE; ++i ) {
    printf("a[%d]=%d\n",i,a[i]);
  }

  return EXIT_SUCCESS;
}

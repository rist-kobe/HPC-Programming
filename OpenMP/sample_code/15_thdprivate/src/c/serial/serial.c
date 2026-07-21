/* Copyright 2024 Research Organization for Information Science and Technology */
#include <stdio.h>
#include <stdlib.h>

int count = 0;

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
    printf("[usage] run.x (arg1)\n  arg1: arary size (int)\n");
    return EXIT_SUCCESS;
  }
  nsize = atoi (argv[1]);
  if ( nsize <= 0 ) {
     return EXIT_FAILURE;
  }

  a = (int *)malloc ( nsize*sizeof(int) );

  for (int i=0; i<nsize; ++i) {
     a[i] = i - nsize/2;
  }

  for (int i=0; i<nsize; ++i ) {
     v = func ( a[i] ) ;
     if ( v < 0 ) {
	inc ();
     }
  }

  printf("Number of negative v:%d\n",count);
  return EXIT_SUCCESS;
}

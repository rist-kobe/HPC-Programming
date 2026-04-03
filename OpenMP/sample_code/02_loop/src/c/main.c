/* Copyright 2024 Research Organization for Information Science and Technology */
#include "mykernel.h"
#include <omp.h>
#include <stdio.h>

#define NSIZE 80000
#define NREP  20000

int main (int argc, char ** argv)
{
  int s;
  double elp, elp0;
  double tmp;
  double a[NSIZE],b[NSIZE],c[NSIZE];

  for (int i=0; i<NSIZE; ++i) {
    a[i] = 0.0;
    b[i] = 1.0;
    c[i] = 2.0;
  }

  elp0 = omp_get_wtime();

  for ( int i=0; i<NREP; ++i ) {
     add(NSIZE,a,b,c);
     s = dummy(NSIZE,a);
     s = s + i;
  }

  elp = omp_get_wtime() - elp0;

  tmp = 0.0;
  for ( int i = 0; i<NSIZE; ++i) 
    tmp += a[i];

  printf("elpase_sec: %10.3f check: %15.8E\n",elp,tmp);

  return 0;
}

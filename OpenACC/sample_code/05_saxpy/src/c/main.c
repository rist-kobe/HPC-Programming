/* Copyright 2024 Research Organization for Information Science and Technology */
#include <stdio.h>
#include <stdlib.h>

#include "mykernel.h"
#include "timer.h"

#define NSIZE 1024
#define NREP  10000

int main (int argc, char ** argv)
{
  int s;
  double elp, elp0;
  float tmp;
  float *x, *y;

  x = (float *)malloc( sizeof(float)*NSIZE );
  y = (float *)malloc( sizeof(float)*NSIZE );

  /* saxpy0 */
  for (int i=0; i<NSIZE; ++i) {
    x[i] = 0.0001f;
  }

  saxpy_0(NSIZE,3.0f,x,y); /* warm up */

  elp0 = get_elp_time();

  s = 0;

  for ( int irep=0; irep<NREP; ++irep ) {
     for (int i=0; i<NSIZE; ++i) {
       y[i] = 0.0f;
     }
     saxpy_0(NSIZE,3.0f,x,y);
     s += dummy(NSIZE,x);
  }

  elp = get_elp_time() - elp0;

  tmp = 0.0f;
  for ( int i = 0; i<NSIZE; ++i) 
    tmp += y[i];

  printf("[SAXPY_0] elpase_sec: %10.3f check: %15.8E\n",elp,tmp);

  /* saxpy1 */
  for (int i=0; i<NSIZE; ++i) {
    x[i] = 0.0001f;
  }

  saxpy_1(NSIZE,3.0f,x,y); /* warm up */

  elp0 = get_elp_time();

  s = 0;

  for ( int irep=0; irep<NREP; ++irep ) {
     for (int i=0; i<NSIZE; ++i) {
       y[i] = 0.0f;
     }
     saxpy_1(NSIZE,3.0f,x,y);
     s += dummy(NSIZE,x);
  }

  elp = get_elp_time() - elp0;

  tmp = 0.0f;
  for ( int i = 0; i<NSIZE; ++i) 
    tmp += y[i];

  printf("[SAXPY_1] elpase_sec: %10.3f check: %15.8E\n",elp,tmp);

  free( x );
  free( y );

  return 0;
}

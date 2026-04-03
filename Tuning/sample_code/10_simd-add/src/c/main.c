/* Copyright 2024 Research Organization for Information Science and Technology */
#include "dummy.h"
#include "mykernel.h"
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

#define NSIZE 32000 
#define NREP 100000

int main (int argc, char **argv)
{
  double va[NSIZE];
  double vb[NSIZE];
  double elp0, elp;

  double tmp = 1.0 / NREP;
  for (int i=0; i<NSIZE; ++i) {
    va[i] = tmp;
    vb[i] = 0.0;
  }
/*======================================================================*/
  printf("%16s %14s %23s\n", "Kernel", "elapsed(sed)", "check");
/*======================================================================*/
  elp0 = get_elp_time();

  for (int i=0; i<NREP; ++i) {
    vadd (NSIZE,va,vb);
    int itmp = dummy (NSIZE,va,vb);
    itmp += i;
  }

  elp = get_elp_time() - elp0;

  tmp = 0.0;
  for (int i=0; i<NSIZE; ++i ) {
    tmp += vb[i]; 
  }

  printf("%16s %14.4f %23.12E\n", "vadd", elp, tmp);

  for (int i=0; i<NSIZE; ++i ) {
     vb[i] = 0.0;
  }
/*======================================================================*/
  elp0 = get_elp_time();

  for (int i=0; i<NREP; ++i) {
    vadd_raw (NSIZE,va,vb);
    int itmp = dummy (NSIZE,va,vb);
    itmp += i;
  }

  elp = get_elp_time() - elp0;

  tmp = 0.0;
  for (int i=0; i<NSIZE; ++i ) {
    tmp += vb[i]; 
  }

  printf("%16s %14.4f %23.12E\n", "vadd_raw", elp, tmp);

  for (int i=0; i<NSIZE; ++i ) {
     vb[i] = 0.0;
  }
/*======================================================================*/
  elp0 = get_elp_time();

  for (int i=0; i<NREP; ++i) {
    vadd_war (NSIZE,va,vb);
    int itmp = dummy (NSIZE,va,vb);
    itmp += i;
  }

  elp = get_elp_time() - elp0;

  tmp = 0.0;
  for (int i=0; i<NSIZE; ++i ) {
    tmp += vb[i]; 
  }

  printf("%16s %14.4f %23.12E\n", "vadd_war", elp, tmp);

  for (int i=0; i<NSIZE; ++i ) {
     vb[i] = 0.0;
  }
/*======================================================================*/
#if defined(_OPENMP)
  elp0 = get_elp_time();

  for (int i=0; i<NREP; ++i) {
    vadd_ompsimd (NSIZE,va,vb);
    int itmp = dummy (NSIZE,va,vb);
    itmp += i;
  }

  elp = get_elp_time() - elp0;

  tmp = 0.0;
  for (int i=0; i<NSIZE; ++i ) {
    tmp += vb[i]; 
  }

  printf("%16s %14.4f %23.12E\n", "vadd_ompsimd", elp, tmp);

  for (int i=0; i<NSIZE; ++i ) {
     vb[i] = 0.0;
  }
#endif
/*======================================================================*/
  return EXIT_SUCCESS;
}

/* Copyright 2024 Research Organization for Information Science and Technology */
#include "dummy.h"
#include "mykernel.h"
#include "mytype.h"
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

int main (int argc, char **argv)
{
  int    ind[NSIZE];

  double va[NSIZE];
  double vb[NSIZE];
  double elp0, elp;

  tcmp   vc[NSIZE];
  tcmpv  vd;

  double tmp = 1.0 / NREP;
  for (int i=0; i<NSIZE; ++i) {
    va[i] = tmp;
    vb[i] = 0.0;
    vc[i].a = tmp;
    vc[i].b = 0.0;
    vd.a[i] = tmp;
    vd.b[i] = 0.0;
  }

  for (int i=0; i<NSIZE; ++i) {
    int ii = i + 46;
    ind[i] = (ii >= NSIZE) ? ii - NSIZE : ii;
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
    vadd_lsta (NSIZE,ind,va,vb);
    int itmp = dummy (NSIZE,va,vb);
    itmp += i;
  }

  elp = get_elp_time() - elp0;

  tmp = 0.0;
  for (int i=0; i<NSIZE; ++i ) {
    tmp += vb[i]; 
  }

  printf("%16s %14.4f %23.12E\n", "vadd_lsta", elp, tmp);

  for (int i=0; i<NSIZE; ++i ) {
     vb[i] = 0.0;
  }
/*======================================================================*/
  elp0 = get_elp_time();

  for (int i=0; i<NREP; ++i) {
    vadd_aos (NSIZE,vc);
    int itmp = dummy (NSIZE,va,vb);
    itmp += i;
  }

  elp = get_elp_time() - elp0;

  tmp = 0.0;
  for (int i=0; i<NSIZE; ++i ) {
    tmp += vc[i].b; 
  }

  printf("%16s %14.4f %23.12E\n", "vadd_aos", elp, tmp);

  for (int i=0; i<NSIZE; ++i ) {
     vc[i].b = 0.0;
  }
/*======================================================================*/
  elp0 = get_elp_time();

  for (int i=0; i<NREP; ++i) {
    vadd_soa (NSIZE,&vd);
    int itmp = dummy (NSIZE,va,vb);
    itmp += i;
  }

  elp = get_elp_time() - elp0;

  tmp = 0.0;
  for (int i=0; i<NSIZE; ++i ) {
    tmp += vd.b[i]; 
  }

  printf("%16s %14.4f %23.12E\n", "vadd_soa", elp, tmp);

  for (int i=0; i<NSIZE; ++i ) {
     vd.b[i] = 0.0;
  }
/*======================================================================*/
  elp0 = get_elp_time();

  for (int i=0; i<NREP; ++i) {
    vadd_while (NSIZE,va,vb);
    int itmp = dummy (NSIZE,va,vb);
    itmp += i;
  }

  elp = get_elp_time() - elp0;

  tmp = 0.0;
  for (int i=0; i<NSIZE; ++i ) {
    tmp += vb[i]; 
  }

  printf("%16s %14.4f %23.12E\n", "vadd_while", elp, tmp);

  for (int i=0; i<NSIZE; ++i ) {
     vb[i] = 0.0;
  }
/*======================================================================*/
  return EXIT_SUCCESS;
}

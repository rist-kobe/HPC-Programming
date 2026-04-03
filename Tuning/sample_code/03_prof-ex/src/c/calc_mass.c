/* Copyright 2024 Research Organization for Information Science and Technology */
/*--------------------------------------------------------------------*/
/* calc_mass.c
   Author: Yukihiro Ota (yota@rist.or.jp)
   Last update: 29 July 2017                                          */
/*--------------------------------------------------------------------*/
#include <stdio.h>
#include "setup.h"
#include "calc.h"

void calc_mass ( InputData *IDp, AtomData *ADp )
{
  for ( int i=0; i<IDp->num_parA ; i=i+1 ) {
    ADp->mass[i] = IDp->mamu_parA;
  }
  for ( int i=IDp->num_parA; i<IDp->num_partot ; i=i+1 ) {
    ADp->mass[i] = IDp->mamu_parB;
  }
  return;
}

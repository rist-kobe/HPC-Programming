/* Copyright 2024 Research Organization for Information Science and Technology */
/*-------------------------------------------------------------------*/
/* calc_inivel.c                                                            
   Author: Yukihiro Ota (yota@rist.or.jp)                            
   Last update: 30 July 2017                                         */
/*-------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include "setup.h"
#include "rndvar.h"
#include "calc.h"

void calc_inivel ( int nn, InputData *IDp, AtomData *ADp )
{

  int icon=1;
  int ird;

  double sigma[nn];
  double rdx[nn],rdy[nn];

  /* dimensionless temperature */
  double tm = IDp->tempt / IDp->temptunit;

  /* variance of the Maxwell distribution with temperature, tm */
  for ( int i=0; i < IDp->num_partot; i=i+1) {
    sigma[i] = sqrt ( tm / ADp->mass[i] ); 
  }

  /* generate random sequence for BM transformation */
  get_rnd_array_for_BM(&nn, rdx, rdy);

  /* initialize Box-Mueller transformation routine */
  boxmueller(&icon, rdx, rdy, &tm);
  
  /* generate velocity following the Maxwell distribution
     with temperature, tm */
  icon = 0;
  for ( int i=0; i < IDp->num_partot; i=i+1){
    tm = sigma[i];
    ird = 3*i;
    ADp->rx[i] = boxmueller(&icon, rdx+ird, rdy+ird, &tm);
    ADp->ry[i] = boxmueller(&icon, rdx+ird+1, rdy+ird+1, &tm);
    ADp->rz[i] = boxmueller(&icon, rdx+ird+2, rdy+ird+2, &tm);
  }

  /* remove excess total momentum */
  {
    double pxtot=0.0;
    double pytot=0.0;
    double pztot=0.0;
    for ( int i=0; i < IDp->num_partot; i=i+1 ) {
      pxtot = pxtot + ADp->mass[i] * ADp->rx[i];
      pytot = pytot + ADp->mass[i] * ADp->ry[i];
      pztot = pztot + ADp->mass[i] * ADp->rz[i];
    }
    pxtot = pxtot / IDp->num_partot;
    pytot = pytot / IDp->num_partot;
    pztot = pztot / IDp->num_partot;
    for ( int i=0; i < IDp->num_partot; i=i+1 ) {
      ADp->rx[i] = ADp->rx[i] - pxtot / ADp->mass[i];
      ADp->ry[i] = ADp->ry[i] - pytot / ADp->mass[i];
      ADp->rz[i] = ADp->rz[i] - pztot / ADp->mass[i];
    }
  }

  return;
}

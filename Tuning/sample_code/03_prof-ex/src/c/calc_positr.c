/* Copyright 2024 Research Organization for Information Science and Technology */
/*--------------------------------------------------------------------*/
/* calc_positr.c                                                            
   Author: Yukihiro Ota (yota@rist.or.jp)                            
   Last update: 30 July 2017                                          */
/*--------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include "setup.h"
#include "calc.h"

/*--------------------------------------------------------------------*/
/* calc_positr1                                                       */
/*--------------------------------------------------------------------*/
void calc_positr1 ( InputData *IDp, AtomData *ADp,
		double x[], double y[], double z[],
		double vx[], double vy[], double vz[] )
{
  /* get input data */
  int ntot     = IDp->num_partot;
  double dtu   = IDp->dtu;
  double dtusq = IDp->dtusq;
  double leng  = IDp->leng;

  /* update by a simple Verlet algorithm */
  for ( int i=0; i < ntot; i=i+1 ) { 
    vx[i] = x[i] + dtu*vx[i] + 0.5*dtusq * ADp->fx[i] / ADp->mass[i]; 
    vy[i] = y[i] + dtu*vy[i] + 0.5*dtusq * ADp->fy[i] / ADp->mass[i]; 
    vz[i] = z[i] + dtu*vz[i] + 0.5*dtusq * ADp->fz[i] / ADp->mass[i]; 
  }

  /* periodic boundary condition (with round in c99) */
  for ( int i=0; i < ntot; i=i+1 ) {
    vx[i] = vx[i] - round( vx[i]/leng )*leng;
    vy[i] = vy[i] - round( vy[i]/leng )*leng;
    vz[i] = vz[i] - round( vz[i]/leng )*leng;
  }

  return;
}
/*--------------------------------------------------------------------*/
/* calc_positr1_diffuse                                               */
/*--------------------------------------------------------------------*/
void calc_positr1_diffuse ( InputData *IDp, AtomData *ADp,
		double x[], double y[], double z[],
		double vx[], double vy[], double vz[] )
{
  /* get input data */
  int ntot     = IDp->num_partot;
  double dtu   = IDp->dtu;
  double dtusq = IDp->dtusq;

  /* update by a simple Verlet algorithm */
  for ( int i=0; i < ntot; i=i+1 ) { 
    vx[i] = x[i] + dtu*vx[i] + 0.5*dtusq * ADp->fx[i] / ADp->mass[i]; 
    vy[i] = y[i] + dtu*vy[i] + 0.5*dtusq * ADp->fy[i] / ADp->mass[i]; 
    vz[i] = z[i] + dtu*vz[i] + 0.5*dtusq * ADp->fz[i] / ADp->mass[i]; 
  }

  return;
}
/*--------------------------------------------------------------------*/
/* calc_positr                                                        */
/*--------------------------------------------------------------------*/
void calc_positr ( InputData *IDp, AtomData *ADp,
		double rbx[], double rby[], double rbz[],
		double rtx[], double rty[], double rtz[])
{
  double rxi,ryi,rzi;

  /* get input data */
  int ntot     = IDp->num_partot;
  double dtusq = IDp->dtusq;
  double leng  = IDp->leng;

  /* perform a simple Verlet algorithm */
  for ( int i=0; i < ntot; i=i+1 ) {
    rxi = 2.0*rtx[i] - rbx[i] + dtusq * ADp->fx[i] / ADp->mass[i]; 
    ryi = 2.0*rty[i] - rby[i] + dtusq * ADp->fy[i] / ADp->mass[i]; 
    rzi = 2.0*rtz[i] - rbz[i] + dtusq * ADp->fz[i] / ADp->mass[i]; 

    rbx[i] = rtx[i];
    rby[i] = rty[i];
    rbz[i] = rtz[i];

    /* periodic boundary condition (with round in c99) */
    rtx[i] = rxi - round( rxi/leng )*leng;
    rty[i] = ryi - round( ryi/leng )*leng;
    rtz[i] = rzi - round( rzi/leng )*leng;
  }

  return;
}
/*--------------------------------------------------------------------*/
/* calc_positr_diffuse                                                */
/*--------------------------------------------------------------------*/
void calc_positr_diffuse ( InputData *IDp, AtomData *ADp,
		double rbx[], double rby[], double rbz[],
		double rtx[], double rty[], double rtz[])
{
  double rxi,ryi,rzi;

  /* get input data */
  int ntot = IDp->num_partot;
  double dtusq   = IDp->dtusq;

  /* perform a simple Verlet algorithm */
  for ( int i=0; i < ntot; i=i+1 ) {
    rxi = 2.0*rtx[i] - rbx[i] + dtusq * ADp->fx[i] / ADp->mass[i]; 
    ryi = 2.0*rty[i] - rby[i] + dtusq * ADp->fy[i] / ADp->mass[i]; 
    rzi = 2.0*rtz[i] - rbz[i] + dtusq * ADp->fz[i] / ADp->mass[i]; 

    rbx[i] = rtx[i];
    rby[i] = rty[i];
    rbz[i] = rtz[i];

    rtx[i] = rxi;
    rty[i] = ryi;
    rtz[i] = rzi;
  }

  return;
}

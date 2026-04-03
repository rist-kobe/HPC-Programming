/* Copyright 2024 Research Organization for Information Science and Technology */
/*--------------------------------------------------------------------*/
/* calc_force.c                                                            
   Author: Yukihiro Ota (yota@rist.or.jp)                            
   Last update: 30 July 2017                                          */
/*--------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include "setup.h"
#include "calc.h"

/*---------------------------------------------------------------------*/
/* calc_force                                                          */
/*---------------------------------------------------------------------*/
void calc_force ( InputData *IDp, AtomData *ADp, 
		double x[], double y[], double z[])
{
  double rtxi,rtyi,rtzi;
  double rtxij,rtyij,rtzij,rtsqij;
  double srt2,srt6,srt12;
  double fxi,fyi,fzi;
  double fxij,fyij,fzij;

  /* LJ parameter */
  const double LJ24 = 24.0;

  /* LJ cutoff distance (dimensionless) */
  const double RC   = 3.0;
  const double RCSQ = 9.0;

  /* get input data */
  int ntot    = IDp->num_partot;
  double leng = IDp->leng;

  /* clear */
  for ( int i=0; i < ntot; i=i+1 ) {
    ADp->fx[i] = 0.0;
    ADp->fy[i] = 0.0;
    ADp->fz[i] = 0.0;
  } 

  /* main loop */
  for ( int ii = 0; ii< ntot-1; ii=ii+1 ) { /* loop on ii */

    rtxi = x[ii];
    rtyi = y[ii];
    rtzi = z[ii];
    fxi  = ADp->fx[ii];
    fyi  = ADp->fy[ii];
    fzi  = ADp->fz[ii];

    for ( int ij = ii+1 ; ij < ntot; ij=ij+1 ) { /* loop on ij */

      rtxij = rtxi - x[ij];
      rtyij = rtyi - y[ij];
      rtzij = rtzi - z[ij];

      /* periodic boundary condition (with round in c99) */
      rtxij = rtxij - round(rtxij/leng)*leng;
      rtyij = rtyij - round(rtyij/leng)*leng;
      rtzij = rtzij - round(rtzij/leng)*leng;

      /* sudden cutoff in potential:
         no force if (inter-atom distance) > RC */
      if ( fabs(rtxij) > RC ) continue;
      if ( fabs(rtyij) > RC ) continue;
      if ( fabs(rtzij) > RC ) continue;

      rtsqij = rtxij*rtxij + rtyij*rtyij + rtzij*rtzij;
      if ( rtsqij > RCSQ ) continue;

      /* calculate force */
      srt2  = 1.0/rtsqij;
      srt6  = pow(srt2,3.0);
      srt12 = pow(srt6,2.0);
      srt2  = LJ24*(2.0*srt12 - srt6)/rtsqij;

      fxij  = srt2*rtxij;
      fyij  = srt2*rtyij;
      fzij  = srt2*rtzij;

      fxi   = fxi + fxij;
      fyi   = fyi + fyij;
      fzi   = fzi + fzij;

      /* action-reaction law */
      ADp->fx[ij] = ADp->fx[ij] - fxij;
      ADp->fy[ij] = ADp->fy[ij] - fyij;
      ADp->fz[ij] = ADp->fz[ij] - fzij;

    } /* End of loop on ij */

    /* update */
    ADp->fx[ii] = fxi;
    ADp->fy[ii] = fyi;
    ADp->fz[ii] = fzi;

  } /* End of loop on ii */

  return;
}
/*--------------------------------------------------------------------*/
/* calc_force_diffuse (no pbc)                                        */
/*--------------------------------------------------------------------*/
void calc_force_diffuse ( InputData *IDp, AtomData *ADp, 
		double x[], double y[], double z[])
{
  double rtxi,rtyi,rtzi;
  double rtxij,rtyij,rtzij,rtsqij;
  double srt2,srt6,srt12;
  double fxi,fyi,fzi;
  double fxij,fyij,fzij;

  /* LJ parameter */
  const double LJ24 = 24.0;

  /* LJ cutoff distance (dimensionless) */
  const double RC   = 3.0;
  const double RCSQ = 9.0;

  /* get input data */
  int ntot = IDp->num_partot;

  /* clear */
  for ( int i=0; i < ntot; i=i+1 ) {
    ADp->fx[i] = 0.0;
    ADp->fy[i] = 0.0;
    ADp->fz[i] = 0.0;
  } 

  /* main loop */
  for ( int ii = 0; ii< ntot-1; ii=ii+1 ) { /* loop on ii */

    rtxi = x[ii];
    rtyi = y[ii];
    rtzi = z[ii];
    fxi  = ADp->fx[ii];
    fyi  = ADp->fy[ii];
    fzi  = ADp->fz[ii];

    for ( int ij = ii+1 ; ij < ntot; ij=ij+1 ) { /* loop on ij */

      rtxij = rtxi - x[ij];
      rtyij = rtyi - y[ij];
      rtzij = rtzi - z[ij];

      /* sudden cutoff in potential:
         no force if (inter-atom distance) > RC */
      if ( fabs(rtxij) > RC ) continue;
      if ( fabs(rtyij) > RC ) continue;
      if ( fabs(rtzij) > RC ) continue;

      rtsqij = rtxij*rtxij + rtyij*rtyij + rtzij*rtzij;
      if ( rtsqij > RCSQ ) continue;

      /* calculate force */
      srt2  = 1.0/rtsqij;
      srt6  = pow(srt2,3.0);
      srt12 = pow(srt6,2.0);
      srt2  = LJ24*(2.0*srt12 - srt6)/rtsqij;

      fxij  = srt2*rtxij;
      fyij  = srt2*rtyij;
      fzij  = srt2*rtzij;

      fxi   = fxi + fxij;
      fyi   = fyi + fyij;
      fzi   = fzi + fzij;

      /* action-reaction law */
      ADp->fx[ij] = ADp->fx[ij] - fxij;
      ADp->fy[ij] = ADp->fy[ij] - fyij;
      ADp->fz[ij] = ADp->fz[ij] - fzij;

    } /* End of loop on ij */

    /* update */
    ADp->fx[ii] = fxi;
    ADp->fy[ii] = fyi;
    ADp->fz[ii] = fzi;

  } /* End of loop on ii */

  return;
}

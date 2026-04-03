/* Copyright 2024 Research Organization for Information Science and Technology */
/*-------------------------------------------------------------------*/
/* calc_iniposit.c                                                            
   Author: Yukihiro Ota (yota@rist.or.jp)                            
   Last update: 31 July 2017                                         */
/*-------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include "setup.h"
#include "rndvar.h"
#include "calc.h"

#ifdef __DEBUG__
#include <stdlib.h>

int calc_iniposit (int nn, InputData *IDp, AtomData *ADp)
{
  int ierr=0;
  char cbuff[2];
  double x,y,z;

  FILE *fp;
  char *filename="geometry.dat";
  fp = fopen(filename,"r");
  for ( int i=0; i < IDp->num_partot; i=i+1 ) {
    fscanf(fp,"%s %lE %lE %lE",cbuff,&x,&y,&z);
    ADp->rx0[i] = x;
    ADp->ry0[i] = y;
    ADp->rz0[i] = z;
  }
  fclose(fp);
  return ierr;
}
#else
int calc_iniposit (int nn, InputData *IDp, AtomData *ADp)
{
  int ierr=0;
  int ii,ij;
  int ix,iy,iz;
  int iok,ird,ioffset,icount;
  const int MAXCOUNT=100;

  double crx0,cry0,crz0;
  double rxij,ryij,rzij,rsqij;
  double rd[nn]; 

  /* get cell length */
  double leng = IDp->leng;

  /* generate random sequence */
  get_rnd_array(&nn,rd);

  /* get position of 1st atom */
  ADp->rx0[0] =  leng * ( rd[0]-0.5 );
  ADp->ry0[0] =  leng * ( rd[1]-0.5 );
  ADp->rz0[0] =  leng * ( rd[2]-0.5 );

  /* generate position of other atoms */
  iok     = 0;
  ii      = 1;
  ird     = 3;
  icount  = 1;
  ioffset = 2;
  while ( ii < IDp->num_partot ) {

    ix  = 3*(ioffset-1);
    iy  = ix  + 1;
    iz  = ix  + 2;
    ird = ird + 3;

    crx0 = leng * ( rd[ix]-0.5 );
    cry0 = leng * ( rd[iy]-0.5 );
    crz0 = leng * ( rd[iz]-0.5 );

    /* check inter-atom distance */
    for ( ij=0; ij < ii; ij=ij+1) {

      rxij = crx0 - ADp->rx0[ij]; 
      ryij = cry0 - ADp->ry0[ij]; 
      rzij = crz0 - ADp->rz0[ij];

      /* periodic boundary condition (with round in c99) */ 
      rxij = rxij - round( rxij/leng )*leng;
      ryij = ryij - round( ryij/leng )*leng;
      rzij = rzij - round( rzij/leng )*leng;

      /* distance criterion: removing too short 
         inter-atom distance */
      rsqij = rxij*rxij + ryij*ryij + rzij*rzij;
      if ( rsqij < 1.0 ) {
        ioffset=ioffset+1;
	iok=1;
        break;
      }
    }

    /* update if iok=0 (and otherwise ... ) */
    if ( iok == 0 ) {
      ADp->rx0[ij] = crx0;
      ADp->ry0[ij] = cry0;
      ADp->rz0[ij] = crz0;
      ii=ii+1;
    } else {
      /* if no remaining random variable */
      if ( ird >= nn-3 ) {
        get_rnd_array(&nn,rd);
	ioffset = 1;
	ird = 0;
	icount = icount + 1;
      }
      iok = 0;
    } 
  
    /* check MAXCOUNT */ 
    if ( icount == MAXCOUNT ) {
      ierr = -1;
      break; 
    } 
  }

  /* finalization */
  if ( ierr < 0 ) {
    printf(
      "Initialization of particle position is failed.\n"
      "This error may often consider a too-dense case.\n"
      "You carefully tune total number of particles and\n"
      "unit-cell size.\n");
     return ierr;
  }

  return ierr;
}
#endif

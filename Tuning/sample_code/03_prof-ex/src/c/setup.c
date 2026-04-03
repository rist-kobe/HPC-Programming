/* Copyright 2024 Research Organization for Information Science and Technology */
/*-------------------------------------------------------------------*/
/* setup.c                                                            
   Author: Yukihiro Ota (yota@rist.or.jp)                            
   Last update: 29 July 2017                                         */
/*-------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "setup.h"
#include "trim.h"
#include "math.h"

/*--------------------------------------------------------------------*/
/* read_input                                                         */
/*--------------------------------------------------------------------*/
int read_input ( InputData *IDp )
{
  FILE *fp;
  char *filename = "./input.dat";
  if ( (fp=fopen(filename,"r")) == NULL ) {
    printf("Input file %s is not found.\n",filename);
    printf("The program is abnormally terminated.\n");
    return -1;
  }

  int maxcbufflen = 128;
  char *cbuff = (char *)malloc(sizeof(char)*maxcbufflen);
  char tag_npT[]  = "<num_partot>";
  char tag_npA[]  = "<num_parA>";
  char tag_npB[]  = "<num_parB>";
  char tag_stp[]  = "<nstep>";
  char tag_obs[]  = "<nobs>";
  char tag_tst[]  = "<iflag_test>";
  char tag_mpA[]  = "<mamu_parA>";
  char tag_mpB[]  = "<mamu_parB>";
  char tag_lcl[]  = "<len_cell>";
  char tag_tem[]  = "<tempt>";
  char tag_eLJ[]  = "<epsLJ>";
  char tag_sLJ[]  = "<sigLJ>";
  char tag_dtm[]  = "<dt>";

  while ( fgets(cbuff,maxcbufflen,fp) != NULL ) {
    if ( strcmp(trim(cbuff),tag_npT) == 0 ) fscanf(fp,"%d",&IDp->num_partot);
    if ( strcmp(trim(cbuff),tag_npA) == 0 ) fscanf(fp,"%d",&IDp->num_parA);
    if ( strcmp(trim(cbuff),tag_npB) == 0 ) fscanf(fp,"%d",&IDp->num_parB);
    if ( strcmp(trim(cbuff),tag_stp) == 0 ) fscanf(fp,"%d",&IDp->nstep);
    if ( strcmp(trim(cbuff),tag_obs) == 0 ) fscanf(fp,"%d",&IDp->nobs);
    if ( strcmp(trim(cbuff),tag_tst) == 0 ) fscanf(fp,"%d",&IDp->iflag_test);
    if ( strcmp(trim(cbuff),tag_mpA) == 0 ) fscanf(fp,"%lf",&IDp->mamu_parA);
    if ( strcmp(trim(cbuff),tag_mpB) == 0 ) fscanf(fp,"%lf",&IDp->mamu_parB);
    if ( strcmp(trim(cbuff),tag_lcl) == 0 ) fscanf(fp,"%lf",&IDp->len_cell);
    if ( strcmp(trim(cbuff),tag_tem) == 0 ) fscanf(fp,"%lf",&IDp->tempt);
    if ( strcmp(trim(cbuff),tag_eLJ) == 0 ) fscanf(fp,"%lf",&IDp->epsLJ);
    if ( strcmp(trim(cbuff),tag_sLJ) == 0 ) fscanf(fp,"%lf",&IDp->sigLJ);
    if ( strcmp(trim(cbuff),tag_dtm) == 0 ) fscanf(fp,"%lf",&IDp->dt);
  }
  free(cbuff);
  fclose(fp);

  int nn = IDp->num_parA + IDp->num_parB - IDp->num_partot;
  if ( 0 != nn ) {
    printf("NTOT must be equal to NA + NB.\n");
    return -1;
  }

  return 0;
}
/*--------------------------------------------------------------------*/
/* write_output_pos.c                                                 */
/*--------------------------------------------------------------------*/
void write_output_pos ( FILE *fp, InputData *IDp,  
		double x[], double y[], double z[])
{
  fprintf(fp,"%8d\n",IDp->num_partot);
  fprintf(fp,"\n");
  for ( int i=0; i < IDp->num_parA; i=i+1 ) {
    fprintf(fp,"%5s %23.8e %23.8e %23.8e\n", "H",x[i],y[i],z[i]);
  }
  for ( int i= IDp->num_parA; i < IDp->num_partot; i=i+1 ) {
    fprintf(fp,"%5s %23.8e %23.8e %23.8e\n", "O",x[i],y[i],z[i]);
  }
}
/*--------------------------------------------------------------------*/
/* set_iparam                                                         */
/*--------------------------------------------------------------------*/
void set_iparam ( InputData *IDp )
{
  IDp->lunit     = IDp->sigLJ;
  IDp->vunit     = sqrt(1.0e-3*IDp->epsLJ/(MAMU*NA));
  IDp->tunit     = 0.10*IDp->lunit/IDp->vunit;
  IDp->temptunit = IDp->epsLJ/KB;
  IDp->ndensunit = 1.0/pow(IDp->lunit,3.0);

  IDp->leng      = IDp->len_cell / IDp->lunit;
  IDp->ndens     = IDp->num_partot / pow(IDp->leng,3.0);

  IDp->dtu       = IDp->dt * 1.0e-6 / IDp->tunit;
  IDp->nstep_upbd= ( 0.50 * IDp->leng 
		  / sqrt(3.0 * IDp->tempt / IDp->temptunit) ) /IDp->dtu;
  IDp->dt_upbd   = 0.10 * IDp->tunit 
	          / sqrt(3.0 * IDp->tempt / IDp->temptunit);
  IDp->dtusq     = IDp->dtu * IDp->dtu;

}
/*--------------------------------------------------------------------*/
/* AtomData_Init                                                      */
/*--------------------------------------------------------------------*/
void AtomData_Init ( AtomData *ADp )
{
  ADp->rx  = NULL;
  ADp->ry  = NULL;
  ADp->rz  = NULL;
  
  ADp->rx0 = NULL;
  ADp->ry0 = NULL;
  ADp->rz0 = NULL;

  ADp->fx  = NULL;
  ADp->fy  = NULL;
  ADp->fz  = NULL;
}
/*--------------------------------------------------------------------*/
/* AtomData_Get_Mem                                                   */
/*--------------------------------------------------------------------*/
void AtomData_Get_Mem ( AtomData *ADp, InputData *IDp )
{
  int n = IDp->num_partot;

  ADp->rx    = (double *)malloc(sizeof(double)*n);
  ADp->ry    = (double *)malloc(sizeof(double)*n);
  ADp->rz    = (double *)malloc(sizeof(double)*n);
  ADp->rx0   = (double *)malloc(sizeof(double)*n);
  ADp->ry0   = (double *)malloc(sizeof(double)*n);
  ADp->rz0   = (double *)malloc(sizeof(double)*n);
  ADp->mass  = (double *)malloc(sizeof(double)*n);
  ADp->fx    = (double *)malloc(sizeof(double)*n);
  ADp->fy    = (double *)malloc(sizeof(double)*n);
  ADp->fz    = (double *)malloc(sizeof(double)*n);
}
/*--------------------------------------------------------------------*/
/* AtomData_Free_Mem                                                  */
/*--------------------------------------------------------------------*/
void AtomData_Free_Mem ( AtomData *ADp )
{
  free ( ADp->rx  );
  free ( ADp->ry  );
  free ( ADp->rz  );
  free ( ADp->rx0 );
  free ( ADp->ry0 );
  free ( ADp->rz0 );
  free ( ADp->mass);
  free ( ADp->fx  );
  free ( ADp->fy  );
  free ( ADp->fz  );
}

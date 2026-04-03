/* Copyright 2024 Research Organization for Information Science and Technology */
#ifndef __SETUP_H__
#define __SETUP_H__
/*--------------------------------------------------------------------*/
/* structure                                                          */
/*--------------------------------------------------------------------*/
typedef struct {

  /* input parameters */
  int num_partot;   /* Total number of particles (NTOT=NA+NB) */ 
  int num_parA;     /* Number of 'A' particle */
  int num_parB;     /* Number of 'B' particle */
  int nstep;        /* Total number of time steps */
  int nobs;         /* Observation step */
  int iflag_test;   /* Flag of a test-run mode */
  double mamu_parA; /* Mass of particle A (atomic mass unit) */
  double mamu_parB; /* Mass of particle B (atomic mass unit) */
  double len_cell;  /* Unit cell length (Angstrom) */
  double tempt;     /* Temperature (K) */
  double epsLJ;     /* epsilon of Lennard-Jones potential (kJ/mol) */
  double sigLJ;     /* sigma of Lennard-Jones potential (Angstrom) */
  double dt;        /* Increment of time step (femto second) */

  /* induced parameters */
  double lunit;     /* unit of length = sigLJ */
  double vunit;     /* unit of velocity = sqrt(epsLJ/MAMU*NA) */
  double tunit;     /* unit of time = sigLJ*sqrt(MAMU*NA/epsLJ) */
  double temptunit; /* unit of temperature = epsLJ/KB */
  double ndensunit; /* unit of number density = sigLJ^-3 */
  double leng;      /* Dimensionless unit cell length */
  double ndens;     /* Dimensionless number density */
  double dtu;       /* Dimensionless time increment */
  double dtusq;     /* dtu^2 */
  double nstep_upbd;/* Upper bound of number of time steps */
  double dt_upbd;   /* Upper bound of time increment */

} InputData;

typedef struct {

  /* position (dimensionless) */
  double *rx;
  double *ry;
  double *rz;
  double *rx0;
  double *ry0;
  double *rz0;

  /* mass (atomic mass unit) */
  double *mass;

  /* force (dimensionless) */
  double *fx;
  double *fy;
  double *fz;

} AtomData;

/*--------------------------------------------------------------------*/
/* function                                                           */
/*--------------------------------------------------------------------*/
int  read_input       ( InputData *IDp );
void write_output_pos ( FILE *fp, InputData *IDp, double x[], 
		double y[], double z[]);
void set_iparam       ( InputData *IDp );
void AtomData_Init    ( AtomData *ADp ); 
void AtomData_Get_Mem ( AtomData *ADp, InputData *IDp ); 
void AtomData_Free_Mem( AtomData *ADp ); 

#endif   /* __SETUP_H__ */
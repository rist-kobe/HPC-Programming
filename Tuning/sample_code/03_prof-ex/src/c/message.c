/* Copyright 2024 Research Organization for Information Science and Technology */
/*--------------------------------------------------------------------*/
/* message.c                                                            
   Author: Yukihiro Ota (yota@rist.or.jp)                            
   Last update: 29 July 2017                                          */
/*--------------------------------------------------------------------*/
#include <stdio.h>
#include "setup.h"
#include "message.h"

/*--------------------------------------------------------------------*/
/* message_of_header                                                  */
/*--------------------------------------------------------------------*/
void message_of_header (void)
{
  printf(
    "======================================================\n"
    " diffuse\n" 
    "MOLECULAR DYNAMICS METHOD FOR MOLECULAR DIFFUSION PROBLEM\n"
    "\n"
    "Author: Yukihiro Ota (yota@rist.or.jp)\n"
    "Last update: 28 July 2017\n"
    "\n"   
    "The original code is taken from a textbook,\n"
    "A. Sato, Introduction to Molecular simulation with sample\n"
    "simulation programs (in Japanese) (Kyouritsu, 2004) 3.1.6\n"
    "\n"  
    "======================================================\n");
  return;
}
/*--------------------------------------------------------------------*/
/* message_of_usage                                                   */
/*--------------------------------------------------------------------*/
void message_of_usage ( void )
{
  message_of_header ();

  printf(
    "-------------------------------------------------------\n"
    "[usage] diffuse.x <arg1>\n"
    "  <arg1>: any integer (e.g. 0)\n"
    "          This argument is never used in the program.\n"
    "-------------------------------------------------------\n"
    "<input parameters>\n"
    "num_partot: Total number of particles (NTOT=NA+NB)\n"
    "num_parA: Number of particle A\n"
    "num_parB: Number of particle B\n"
    "nstep: Total number of time steps\n"
    "nobs: Observation step\n"
    "\n"
    "iflag_test: Flag for test mode (0/1)\n"
    "  0: test mode (no real calculation is performed)\n"
    "  1: simulation mode\n"
    "\n"
    "mamu_parA: Mass of particle A (atomic mass unit)\n"
    "mamu_parB: Mass of particle B (atomic mass unit)\n"
    "len_cel: Unit-cell length (square box) (Angstrom)\n"
    "tempt: Temperature (K)\n"
    "epsLJ: epsilon of LJ pot (kJ/mol)\n"
    "sigLJ: sigma of LJ pot (Angstrom)\n"
    "dt: Increment of time step (femto second)\n"
    "\n"); 
  return;
}
/*--------------------------------------------------------------------*/
/* message_of_rephinp                                                 */
/*--------------------------------------------------------------------*/
void message_of_rephinp ( InputData *pt )
{

  puts("[Input parameters]");
  printf("num_partot: Total number of particles (NTOT=NA+NB)\n%10d\n",
		  pt->num_partot);
  printf("num_parA: Number of particle A\n%10d\n",pt->num_parA);
  printf("num_parB: Number of particle B\n%10d\n",pt->num_parB);
  printf("nstep: Total number of time steps\n%10d\n",pt->nstep);
  printf("nobs: Observation step\n%10d\n",pt->nobs);
  printf("\n");
       
  printf("iflag_test: Flag for test mode (0/1)\n%10d\n",
		  pt->iflag_test);
  printf("mamu_parA: Mass of particle A \n%17.6e%18s\n",
		  pt->mamu_parA,"amu");
  printf("mamu_parB: Mass of particle B \n%17.6e%18s\n",
		  pt->mamu_parB,"amu");
  printf("len_cell: Unit-cell length (square box) \n%17.6e%18s\n",
		  pt->len_cell,"Angstrom");
  printf("tempt: Temperature \n%17.6e%18s\n",pt->tempt,"K");
  printf("epsLJ: epsilon of LJ pot \n%17.6e%18s\n",pt->epsLJ,"kJ/mol");
  printf("sigLJ: sigma of LJ pot \n%17.6e%18s\n",pt->sigLJ,"Angstrom");
  printf("dt: Increment of time step \n%17.6e%18s\n",pt->dt,"fs");
  printf("\n");

  return;
}
/*--------------------------------------------------------------------*/
/* message_of_calcparam                                               */
/*--------------------------------------------------------------------*/
void message_of_calcparam ( InputData *pt )
{

  puts("[Calculated parameters]");
  printf("<Unit of length>\n%17.6e%18s\n",pt->lunit, "(Angstrom)");
  printf("<Unit of velocity>\n%17.6e%18s\n",pt->vunit, "(m/sec)");
  printf("<Unit of time>\n%17.6e%18s\n",pt->tunit, "(ns)");
  printf("<Unit of temperature>\n%17.6e%18s\n",pt->temptunit, "(K)");
  printf("<Unit of number density>\n%17.6e%18s\n",
		  pt->ndensunit, "(Angstrom)^-3");
  printf("\n");
       
  printf("<Dimensionless unit-cell length>\n%17.6e\n",pt->leng);
  printf("<Dimensionless number density>\n%17.6e\n",pt->ndens);
  printf("<Dimensionless time increment>\n%17.6e\n",pt->dtu);
  printf("\n");
       
  printf("The following two quantities are useful for\n"
         "tuning <nstep> and <dt>.\n");
  printf("<Characteristic number of time steps>\n"
         "(unit-cell-length/2)/(dt*sqrt(3*temperature))=\n");
  printf("%17.6e\n",pt->nstep_upbd);
  printf("<Characteristic dimensionless time increment>\n"
         "0.1/sqrt(3*temperature)=\n");
  printf("%17.6e\n",pt->dt_upbd);
  printf("\n");
  return;
}
/*--------------------------------------------------------------------*/
/* message_of_output                                                  */
/*--------------------------------------------------------------------*/
void message_of_output ( void ) 
{
  printf(
    "[Explanation on output files]\n"
    "+ inigeom.xyz\n"
    "Initial configuration of particle positions is shown\n"
    "with the XYZ format. Each coordinate is dimensionless.\n"
    "\n"
    "+ trj.xyz\n"
    "Trajectory of particle positions is shown\n"
    "with the XYZ format. Each coordinate is dimensionless\n"
    "The stepsize of the trajectory is controlled by the \n"
    "input parameter, <nobs>.\n"
    "\n");
  return;
}
/*--------------------------------------------------------------------*/
/* message_of_testmode                                                */
/*--------------------------------------------------------------------*/
void message_of_testmode ( InputData *p ) 
{

  message_of_header ();

  message_of_rephinp ( p );

  printf("--TEST-MODE-TEST-MODE-TEST-MODE-TEST-MODE-TEST-MODE--\n");

  message_of_calcparam ( p ); 

  message_of_output ();

  return;
}
/*--------------------------------------------------------------------*/
/* message_of_abtermination                                           */
/*--------------------------------------------------------------------*/
void message_of_abtermination ( void )
{
   printf("*** The simulation is abnormally terminated ***\n");
   return;
}

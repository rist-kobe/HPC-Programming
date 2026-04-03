/* Copyright 2024 Research Organization for Information Science and Technology */
/*----------------------------------------------------------------------
 *  Check resolution of your timer 
 *  Author:      Yukihiro Ota (yota@rist.or.jp)
 *  Description: The original idea comes from Wadleigh and Crawford,
 *               "Software Optimization for HPC: Creating Faster 
 *                Applications" (2000) pp.136-138 
 *--------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

int func ( const int nn ) ;

/*--------------------------------------------------------------------*/
/*  main                                                              */
/*--------------------------------------------------------------------*/
int main ( int argc, char* argv[] )
{
  int nn ;
  double t1, t2 ;

  printf ("--------------------------------------------------------\n");

  /* check resolution of wallclock timer */
  t2 = -1.0 ;
  nn = 0 ;
  while ( t2 <= 0.0 ) {
    nn++ ;
    t1 = get_elp_time () ; 
    func ( nn ) ;
    t2 = get_elp_time () ;
    t2 -= t1 ;
  }
  printf ("[Check resolution of wallclock timer]\n") ;
  printf ("It took %7d iterations to generate a none-zero time\n", nn) ;
  if ( nn == 1 ) {
    printf (" timer resolution less than or equal to %15.9f\n", t2) ;      
  } else {
    printf (" timer resolution is %15.9f sec.\n", t2) ;      
  }

  printf ("--------------------------------------------------------\n");

  /* check resolution of cpu timer */
  t2 = -1.0 ;
  nn = 0 ;
  while ( t2 <= 0.0 ) {
    nn++ ;
    t1 = get_cpu_time () ; 
    func ( nn ) ;
    t2 = get_cpu_time () ;
    t2 -= t1 ;
  }
  printf ("[Check resolution of cpu timer]\n") ;
  printf ("It took %7d iterations to generate a none-zero time\n", nn) ;
  if ( nn == 1 ) {
    printf (" timer resolution less than or equal to %15.9f\n", t2) ;      
  } else {
    printf (" timer resolution is %17.9f sec.\n", t2) ;      
  }

  printf ("--------------------------------------------------------\n");

  /* finalization */
  return EXIT_SUCCESS ;
}
/*--------------------------------------------------------------------*/
/*  func                                                              */
/*--------------------------------------------------------------------*/
int func ( const int nn ) 
{
  int i, j ;
  i = 0 ;
  for ( j = 0 ; j < nn ; ++j ) i++ ;
  return i ;
}

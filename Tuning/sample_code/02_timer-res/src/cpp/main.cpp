// Copyright 2024 Research Organization for Information Science and Technology 
//----------------------------------------------------------------------
//  Check resolution of your timer 
//  Author:      Yukihiro Ota (yota@rist.or.jp)
//  Description: The original idea comes from Wadleigh and Crawford,
//               "Software Optimization for HPC: Creating Faster 
//                Applications" (2000) pp.136-138 
//----------------------------------------------------------------------
#include <chrono>
#include <cstdio>
#include <cstdlib>
//#include <iostream>

int func ( const int nn ) ;

//----------------------------------------------------------------------
//  main                                                              
//----------------------------------------------------------------------
int main ( int argc, char* argv[] )
{
  int nn ;
  double t ;

  printf ("--------------------------------------------------------\n");

  // check resolution of wallclock timer with steady_clock
  nn = 0;
  t = 0.0;
  while ( t <= 0.0 ) {
    nn++;
    const auto elp1 = std::chrono::steady_clock::now();
    func (nn);
    const auto elp2 = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed = elp2 - elp1;
    t = static_cast<double>(elapsed.count());
    //std::cout << elapsed.count() << std::endl;
    //std::cout << t << std::endl;
  }

  printf ("[Check resolution of steady_clock]\n") ;
  printf ("It took %7d iterations to generate a none-zero time\n", nn) ;
  if ( nn == 1 ) {
    printf (" timer resolution less than or equal to %15.9f\n", t) ;      
  } else {
    printf (" timer resolution is %15.9f sec.\n", t) ;      
  }

  printf ("--------------------------------------------------------\n");

  // check resolution of wallclock timer with high_resolution_clock
  nn = 0;
  t = 0.0;
  while ( t <= 0.0 ) {
    nn++;
    const auto elp1 = std::chrono::high_resolution_clock::now();
    func (nn);
    const auto elp2 = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed = elp2 - elp1;
    t = static_cast<double>( elapsed.count() );
  }

  printf ("[Check resolution of high_resolution_clock]\n") ;
  printf ("It took %7d iterations to generate a none-zero time\n", nn) ;
  if ( nn == 1 ) {
    printf (" timer resolution less than or equal to %15.9f\n", t) ;      
  } else {
    printf (" timer resolution is %15.9f sec.\n", t) ;      
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

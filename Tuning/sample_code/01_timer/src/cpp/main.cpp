// Copyright 2024 Research Organization for Information Science and Technology 
//----------------------------------------------------------------------
//   By-hand timer (C++11) and Use of gprof 
//   Author:      Yukihiro Ota (yota@rist.or.jp)
// ---------------------------------------------------------------------
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
//#include <thread>

void sub1 ( double *a, const int nn ) ;
void sub2 ( double *a, const int nn ) ;
void sub3 ( double *a, const int nn ) ;
//void sub4 ( );

//----------------------------------------------------------------------
//  main                                                              
//----------------------------------------------------------------------
int main ( int argc, char* argv[] )
{
  int nn = 20000 ;
  double a[nn] ;

  // set values 
  for ( int i=0; i<nn; ++i ) a[i] = 0.0 ;

  // routine 1 
#if defined(USE_TIMER)
  {
     const auto elp1 = std::chrono::steady_clock::now();

     for ( int k=0; k<100000; ++k ) sub1 ( a, nn ) ;

     const auto elp2 = std::chrono::steady_clock::now();
     const std::chrono::duration<double> elapsed = elp2 - elp1;

     std::cout << "Elapsed time (sec)   = " << elapsed.count() << std::endl;
     // If you perfer to cstdio
     // fprintf(stdout, "Elapsed time (seconds) : %9.3f\n", static_cast<double>(elapsed.count()));
  }
#else
  for ( int k=0; k<100000; ++k ) sub1 ( a, nn ) ;
#endif

  fprintf (stdout, "a[0] = %13.6f\n", a[0] ) ;

  // routine 2 
#if defined(USE_TIMER)
  {
     const auto elp1 = std::chrono::steady_clock::now();

     for ( int k=0; k<200000; ++k ) sub2 ( a, nn ) ;

     const auto elp2 = std::chrono::steady_clock::now();
     const std::chrono::duration<double> elapsed = elp2 - elp1;

     std::cout << "Elapsed time (sec)   = " << elapsed.count() << std::endl;
     // If you perfer to cstdio
     // fprintf(stdout, "Elapsed time (seconds) : %9.3f\n", static_cast<double>(elapsed.count()));
  }
#else
  for ( int k=0; k<200000; ++k ) sub2 ( a, nn ) ;
#endif

  fprintf (stdout, "a[0] = %13.6f\n", a[0] ) ;

  // finalization 
  return EXIT_SUCCESS ;
}
//----------------------------------------------------------------------
//  sub1                                                              
//----------------------------------------------------------------------
void sub1 ( double *a, const int nn ) {

  for ( int i=0; i<nn; ++i ) a[i] += 1.0 ;

  for ( int k=0; k<2; ++k ) sub3( a, nn ) ;

}
//----------------------------------------------------------------------
//  sub2                                                              
//----------------------------------------------------------------------
void sub2 ( double *a, const int nn ) {

  a[0] = sin(a[0]) ;

  for ( int i=0; i<nn; ++i ) a[i] += 1.0 ;

  for ( int k=0; k<4; ++k ) sub3( a, nn ) ;

}
//----------------------------------------------------------------------
//  sub3                                                              
//----------------------------------------------------------------------
void sub3 ( double *a, const int nn ) {

  a[0] = sin(a[0]) ;

  for ( int i=0; i<nn; ++i ) a[i] += 1.0 ;

}
//----------------------------------------------------------------------
//  sub4                                                              
//----------------------------------------------------------------------
//void sub4 ( ) {
//  // sleep 
//  std::this_thread::sleep_for(std::chrono::seconds(30)); 
//}

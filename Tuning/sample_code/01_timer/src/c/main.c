/* Copyright 2024 Research Organization for Information Science and Technology */
/*----------------------------------------------------------------------
 *  By-hand timer and Use of gprof 
 *  Author:      Yukihiro Ota (yota@rist.or.jp)
 *--------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"

void sub1 ( double *a, const int nn ) ;
void sub2 ( double *a, const int nn ) ;
void sub3 ( double *a, const int nn ) ;

/*--------------------------------------------------------------------*/
/*  main                                                              */
/*--------------------------------------------------------------------*/
int main ( int argc, char* argv[] )
{
  int nn = 20000 ;
  double a[nn] ;
#if defined(USE_CPU_TIMER)
  double cpu1, cpu2;
#endif
#if defined(USE_ELP_TIMER)
  double elp1, elp2;
#endif

  /* check resolution of timer */
/*#if defined(USE_CPU_TIMER)
  cpu1 = get_cpu_res();
  printf ( "Res. of timer (micro sec) = %17.6e\n", cpu1) ;
#endif
#if defined(USE_ELP_TIMER)
  elp1 = get_elp_res(); 
  printf ( "Res. of timer (micro sec) = %17.6e\n", elp1) ;
#endif*/

  /* set values */
  for ( int i=0; i<nn; ++i ) a[i] = 0.0 ;

  /* routine 1 */
#if defined(USE_CPU_TIMER)
  cpu1 = get_cpu_time () ; // cpu time
#endif
#if defined(USE_ELP_TIMER)
  elp1 = get_elp_time () ; // wall clock
#endif

  for ( int k=0; k<100000; ++k ) sub1 ( a, nn ) ;

#if defined(USE_CPU_TIMER)
  cpu2 = get_cpu_time () ; // cpu time
  printf ( "CPU time (sec)     = %17.6f\n", cpu2 - cpu1 ) ;
#endif
#if defined(USE_ELP_TIMER)
  elp2 = get_elp_time () ; // wall clock
  printf ( "Elapsed time (sec) = %17.6f\n", elp2 - elp1 ) ;
#endif 

  printf ("a[0] = %13.6f\n", a[0] ) ;

  /* routine 2 */
#if defined(USE_CPU_TIMER)
  cpu1 = get_cpu_time () ; // cpu time
#endif
#if defined(USE_ELP_TIMER)
  elp1 = get_elp_time () ; // wall clock
#endif

  for ( int k=0; k<200000; ++k ) sub2 ( a, nn ) ;

#if defined(USE_CPU_TIMER)
  cpu2 = get_cpu_time () ; // cpu time
  printf ( "CPU time (sec)     = %17.6f\n", cpu2 - cpu1 ) ;
#endif
#if defined(USE_ELP_TIMER)
  elp2 = get_elp_time () ; // wall clock
  printf ( "Elapsed time (sec) = %17.6f\n", elp2 - elp1 ) ;
#endif

  printf ("a[0] = %13.6f\n", a[0] ) ;

  /* finalization */
  return EXIT_SUCCESS ;
}
/*--------------------------------------------------------------------*/
/*  sub1                                                              */
/*--------------------------------------------------------------------*/
void sub1 ( double *a, const int nn ) {

  for ( int i=0; i<nn; ++i ) a[i] += 1.0 ;

  for ( int k=0; k<2; ++k ) sub3( a, nn ) ;

}
/*--------------------------------------------------------------------*/
/*  sub2                                                              */
/*--------------------------------------------------------------------*/
void sub2 ( double *a, const int nn ) {

  a[0] = sin(a[0]) ;

  for ( int i=0; i<nn; ++i ) a[i] += 1.0 ;

  for ( int k=0; k<4; ++k ) sub3( a, nn ) ;

}
/*--------------------------------------------------------------------*/
/*  sub3                                                              */
/*--------------------------------------------------------------------*/
void sub3 ( double *a, const int nn ) {

  a[0] = sin(a[0]) ;

  for ( int i=0; i<nn; ++i ) a[i] += 1.0 ;

}

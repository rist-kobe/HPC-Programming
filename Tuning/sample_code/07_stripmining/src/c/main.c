/* Copyright 2024 Research Organization for Information Science and Technology */
/*---------------------------------------------------------------------
  Strip mining
  Author:       Yukihiro Ota (yota@rist.or.jp)
  Last update:  28 Aug 2018
----------------------------------------------------------------------*/
#include<stdio.h>
#include<stdlib.h>
#include"timer.h"

#define MIN( a, b ) ( (a) < (b) ? (a) : (b) )
#define MAX( a, b ) ( (a) < (b) ? (b) : (a) )

void addmulti(double* restrict, double, const int, const int) ;
void addmulti_stripmining(double* restrict, double, const int, 
  const int, const int) ;

int main ( int argc, char **argv )
{
  int nlarge, nsmall, veclen ;
  const int totitr = 10000000 ;

  double stmp1, stmp2 ;
  double *a ;
  double b ;

  /*-------------------------------------------------------------------
      parse command line options
  --------------------------------------------------------------------*/
  if ( argc != 4 ) {
    printf("[usage] run.x (arg1) (arg2) (arg3)\n") ;
    printf("  arg1: 1st loop length (int)\n") ;
    printf("  arg2: 2nd loop length (int)\n") ;
    printf("  arg3: vector length of strip-mining (int)\n") ;
    printf("  requirement: (arg2) < (arg1)\n") ;
    printf("  requirement: (arg3) < (arg2)\n") ;
    return EXIT_SUCCESS ;
  }

  nlarge = atoi(argv[1]) ;
  nsmall = atoi(argv[2]) ;
  veclen = atoi(argv[3]) ;

  if ( nlarge < nsmall ) {
    printf("Error: 2nd arg must be smaller than 1st one.\n") ;
    return EXIT_FAILURE ; 
  }

  if ( nsmall < veclen ) {
    printf("Error: 3rd arg must be smaller than 2nd one.\n") ;
    return EXIT_FAILURE ; 
  }

  printf("[Job setting]\n") ;
  printf("1st loop length %d\n", nlarge) ;
  printf("2nd loop length %d\n", nsmall) ;
  printf("vec length of strip-mining %d\n", veclen) ;

  /*-------------------------------------------------------------------
     Addition loops and Multiplication loop 
  --------------------------------------------------------------------*/
  a = (double *)malloc( sizeof(double)*nlarge ) ;

  for ( int i = 0; i < nlarge ; ++i ) 
    a[i] = ((double)(i-nlarge/2))*1.0/nlarge ;

  b = 0.0 ;

  stmp1 = get_cpu_time() ;
  for ( int itr = 1; itr < totitr; ++itr) {
    addmulti(a, b, nlarge, nsmall) ;
    b = ((double)itr) / totitr ;
  }
  stmp2 = get_cpu_time() ;

  printf("Standard\n") ;
  printf("CPU time (sec) %13.6f\n", stmp2 - stmp1) ;
  printf("a[0] = %15.6e\n", a[0]) ;

  free ( a ) ;
  /*-------------------------------------------------------------------
     Strip minging: Addition loops and Multiplication loop 
  --------------------------------------------------------------------*/
  a = (double *)malloc( sizeof(double)*nlarge ) ;

  for ( int i = 0; i < nlarge ; ++i ) 
    a[i] = ((double)(i-nlarge/2))*1.0/nlarge ;

  b = 0.0 ;

  stmp1 = get_cpu_time() ;
  for ( int itr = 1; itr < totitr; ++itr) {
    addmulti_stripmining(a, b, nlarge, nsmall, veclen) ;
    b = ((double)itr) / totitr ;
  }
  stmp2 = get_cpu_time() ;

  printf("Stripmining with vector length %d\n", veclen) ;
  printf("CPU time (sec) %13.6f\n", stmp2 - stmp1) ;
  printf("a[0] = %15.6e\n", a[0]) ;
  
  free ( a ) ; 
  /*------------------------------------------------------------------
     Finalization
  --------------------------------------------------------------------*/
  return EXIT_SUCCESS ;
}
/*--------------------------------------------------------------------
   Addition loops and Multiplication loop
----------------------------------------------------------------------*/
void addmulti(double* restrict a, double b, const int nl, const int ns)
{
   for ( int i = 0; i < nl ; ++i )
      a[i] += b ;

   for ( int i = 0; i < ns ; ++i )
      a[i] *= b ;
}
/*-------------------------------------------------------------------
  Strip mining: Addition loop and Multiplication loop 
---------------------------------------------------------------------*/
void addmulti_stripmining(double* restrict a, double b, const int nl, 
  const int ns, const int vlen)
{
   for ( int i = 0 ; i < nl ; i+=vlen ) {

     /* if vlen elements are on cache, the data is reusable
      * in the two successive procedures (add and multi) */  
     for ( int ii = i ; ii < MIN(i+vlen,nl) ; ++ii ) 
       a[ii] += b ;
     for ( int ii = i ; ii < MIN(i+vlen,ns) ; ++ii ) 
       a[ii] *= b ;

   }
}

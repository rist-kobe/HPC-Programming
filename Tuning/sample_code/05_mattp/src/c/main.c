/* Copyright 2024 Research Organization for Information Science and Technology */
/*---------------------------------------------------------------------
 *  Loop blocking: Matrix transpose 
 *  Author:      Yukihiro Ota (yota@rist.or.jp)
 *  Reference:   See Sec.3.4 in Introduction to High Performance 
 *               Computing for Scientists and Engineers by 
 *               Hager and Wellein (CRC Press, 2011).
 *-------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

#define MIN(a, b) ( (a) < (b) ? (a) : (b) )

void mattp ( double *a, double *a_tp, const int nr, const int nc, 
             int it, int *idm ) ; 
void mattp_bk ( double *a, double *a_tp, const int nr, const int nc, 
                int it, int *idm, const int nbk ) ; 

int main ( int argc, char* argv[] ) 
{
  int nrow, ncol, nbk, nsize, nrbk, ncbk;
  int idm ;

  double mema ;
  double *a ;
  double *b ;

#ifdef __USE_CPU_TIMER__
  double elpsta, elp[2] ;
#endif

  /*-------------------------------------------------------------------
   * get matrix row size and block size from arguments 
   *-----------------------------------------------------------------*/
  if ( argc != 3 ) {
    printf ("[usage] run.x $1 $2\n") ;
    printf ("        $1: matrix row size (integer)\n") ;
    printf ("        $2: block size      (integer)\n") ;
    return EXIT_SUCCESS ;
  }
  // argument 1
  nrow = atoi (argv[1]) ;
  ncol = nrow ;
  if ( nrow <= 0 ) {
    printf ("Matrix dimension must be strictly positive\n") ;
    printf ("The program is terminated\n") ;
    return EXIT_FAILURE ;
  }
  if ( nrow > 8192 ) {
    /* 2 double arrays * 8 bytes * (nrow)^2 < 1 GiB=2^30 bytes */
    printf ("Matrix row size must be smaller than 8192\n") ;
    printf ("The program is terminated\n") ;
    return EXIT_FAILURE ;
  } 
  // argument 2
  nbk = atoi (argv[2]) ;
  if ( nbk <= 0 ) {
    printf ("Block size must be strictly positive\n") ;
    printf ("The program is terminated\n") ;
    return EXIT_FAILURE ;
  }
  if ( nbk >= nrow ) {
    printf ("Block size must be smaller than matrix row size\n") ;
    printf ("The program is terminated\n") ;
    return EXIT_FAILURE ;
  }
  nrbk = (nrow + (nbk - 1) ) /nbk ; // Block num. (rounded up)
  ncbk = (ncol + (nbk - 1) ) /nbk ; // Block num. (rounded up)
  /*-------------------------------------------------------------------
   * memory allocation  (and obtain physical memory)
   *-----------------------------------------------------------------*/
  nsize = nrow * ncol ;
  a = (double *)malloc ( sizeof(double)*nsize ) ;
  b = (double *)malloc ( sizeof(double)*nsize ) ;
  {
    double wk = 1.0/nsize ;
    int hnsize = nsize / 2 ; //use of bitwise op would be preferable .. 
    for ( int i=0; i<nrow; ++i ) {
      for ( int j=0; j<ncol; ++j ) {
        int n = j + i*ncol ;
	a[n] = 0.0 ;
        b[n] = 1.0 + (n-hnsize)*wk ; 
      }
    }
  }
  /*-------------------------------------------------------------------
   * matrix transpose 
   *-----------------------------------------------------------------*/
  mema = 2.0*sizeof(double)*nsize*1.0e-6 ; // MB
  idm = 0 ;
#ifdef __USE_CPU_TIMER__
  elpsta = get_cpu_time () ; 
#endif
  for ( int itr=0; itr<10000; ++itr) {
    mattp ( b, a, nrow, ncol, itr, &idm ) ; // a = ^t b
    idm += 1 ;
  }
#ifdef __USE_CPU_TIMER__
  elp[0] = get_cpu_time () ; 
  elp[0] = elp[0] - elpsta ;
#endif
  /*-------------------------------------------------------------------
   * standard output 
   *-----------------------------------------------------------------*/
  printf ("Matrix transpose (standard)    \n") ;
  printf ("Number of iterations           \n") ;
  printf ("%d\n", idm ) ;
  printf ("Check matrix transpose         \n") ;
  printf ("%d %d %17.6f %17.6f\n", 
          0, ncol-1, a[ncol-1], b[ncol-1]);
  printf ("%d %d %17.6f %17.6f\n", 
          nrow-1, 0, a[(nrow-1)*ncol], b[(nrow-1)*ncol]);
  printf ("Req. memory (MB)               \n") ;
  printf ("%17.6f\n", mema) ;
#ifdef __USE_CPU_TIMER__
  printf ("CPU time (sec)                 \n") ;
  printf ("%17.6f\n", elp[0]) ;
#endif
  /*-------------------------------------------------------------------
   * memory delete and allocation  (and obtain physical memory)
   *-----------------------------------------------------------------*/
  free ( a ); a = NULL;
  free ( b ); b = NULL;
  a = (double *)malloc ( sizeof(double)*nsize ) ;
  b = (double *)malloc ( sizeof(double)*nsize ) ;
  {
    double wk = 1.0/nsize ;
    int hnsize = nsize / 2 ; //use of bitwise op would be preferable .. 
    for ( int i=0; i<nrow; ++i ) {
      for ( int j=0; j<ncol; ++j ) {
        int n = j + i*ncol ;
	a[n] = 0.0 ;
        b[n] = 1.0 + (n-hnsize)*wk ; 
      }
    }
  }
  /*-------------------------------------------------------------------
   * matrix transpose (blocking)
   *-----------------------------------------------------------------*/
  mema = 2.0*sizeof(double)*nsize*1.0e-6 ; // MB
  idm = 0 ;
#ifdef __USE_CPU_TIMER__
  elpsta = get_cpu_time () ; 
#endif
  for ( int itr=0; itr<10000; ++itr) {
    mattp_bk ( b, a, nrow, ncol, itr, &idm, nbk ) ; // a = ^t b
    idm += 1 ;
  }
#ifdef __USE_CPU_TIMER__
  elp[1] = get_cpu_time () ; 
  elp[1] = elp[1] - elpsta ;
#endif
  /*-------------------------------------------------------------------
   * standard output 
   *-----------------------------------------------------------------*/
  printf ("Matrix transpose (blocking)    \n") ;
  printf ("Number of iterations           \n") ;
  printf ("%d\n", idm ) ;
  printf ("Check matrix transpose         \n") ;
  printf ("%d %d %17.6f %17.6f\n", 
          0, ncol-1, a[ncol-1], b[ncol-1]);
  printf ("%d %d %17.6f %17.6f\n", 
          nrow-1, 0, a[(nrow-1)*ncol], b[(nrow-1)*ncol]);
  printf ("Req. memory (MB)               \n") ;
  printf ("%17.6f\n", mema) ;
  printf ("Number of blocks w.r.t.        \n") ;
  printf ("   row: %d  column: %d\n", nrbk, ncbk ) ;
#ifdef __USE_CPU_TIMER__
  printf ("CPU time (sec)                 \n") ;
  printf ("%17.6f\n", elp[1]) ;
#endif
  /*-------------------------------------------------------------------
   * finalization  
   *-----------------------------------------------------------------*/
  free ( a ) ; free ( b ) ;

  return EXIT_SUCCESS ;
}
/*---------------------------------------------------------------------
 * mattp
 *-------------------------------------------------------------------*/
void mattp ( double *a, double *a_tp, const int nr, const int nc, 
             int it, int *idm ) {

  for ( int i=0; i<nr; ++i ) {
  for ( int j=0; j<nc; ++j ) {
      int id  = j + i*nc ; 
      int idt = i + j*nr ;  // stride nr access on loop j
      a_tp[id] = a[idt] ;   // 1 load, 1 store, no FP operation
  }}

  *idm = it ;
}     
/*---------------------------------------------------------------------
 * blocked mattp
 *-------------------------------------------------------------------*/
void mattp_bk ( double *a, double *a_tp, const int nr, const int nc, 
                int it, int *idm, const int nbk ) {

  for ( int ii=0; ii<nr; ii+=nbk ) { 
    int iend = MIN(ii+nbk,nr) ; 
  for ( int jj=0; jj<nc; jj+=nbk ) {
    int jend = MIN(jj+nbk,nc) ; 

    for ( int i=ii; i<iend; ++i ) { 
    for ( int j=jj; j<jend; ++j ) {
       int id  = j + i*nc ; // What about stride? 
       int idt = i + j*nr ; // What about stride?
       a_tp[id] = a[idt] ;     // 1 load, 1 store, no FP operation
    }}

  }}

  *idm = it ;
}     


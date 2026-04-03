/* Copyright 2024 Research Organization for Information Science and Technology */
/*----------------------------------------------------------------------
 * Loop unrolling on an outer loop
 * Author:      Yukihiro Ota (yota@rist.or.jp)
 * Description: related to Sec.5.5 in the textbook
 *--------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"

void fma0 ( double *a, double *b, double *c, 
           const int nout, const int nin, const int itr, int *idm ) ;
void fma4_nf ( double *a, double *b, double *c, 
           const int nout, const int nin, const int itr, int *idm ) ;
void fma4 ( double *a, double *b, double *c, 
           const int nout, const int nin, const int itr, int *idm ) ;

int main ( int argc, char** argv ) 
{
  int nout, nin, ntot ;
  int idm ;

  double res ;
  double *a ;
  double *b ;
  double *c ;
  double *aref ;

#ifdef __USE_CPU_TIMER__
  double elpst, elp[3] ;
#endif
  /*--------------------------------------------------------------------
   *  get outer and inner loop lengths from arguments
   *------------------------------------------------------------------*/
  if ( argc != 3 ) {
    printf ("[usage] ./run.x $1 $2\n") ;
    printf ("   $1: size of outer loop (integer)\n") ;
    printf ("   $2: size of inner loop (integer)\n") ;
    return EXIT_SUCCESS ;
  }
  nout = atoi ( argv[1] ) ;
  nin  = atoi ( argv[2] ) ;
  ntot = nout*nin ;
  if ( nout < 4 ) {
    printf ("Outer loop size must be greater than 4.\n") ;
    printf ("The program is terminated.\n") ;
    return EXIT_FAILURE ;
  }
  if ( nin <= 0 ) {
    printf ("Inner loop size must be greater than 0.\n") ;
    printf ("The program is terminated.\n") ;
    return EXIT_FAILURE ;
  }
  if ( ntot > 8192*8192 ) {
    /* ntot*8bytes < 512 MiB=2^29 */
    printf ("(outer loop)*(inner loop) < 8192*8192  \n") ;
    printf ("The program is terminated.\n") ;
    return EXIT_FAILURE ;
  }
  /*--------------------------------------------------------------------
   *  memory allocation 
   *------------------------------------------------------------------*/
  a    = (double *)malloc ( nin*sizeof(double) ) ;
  b    = (double *)malloc ( nin*sizeof(double) ) ;
  c    = (double *)malloc ( ntot*sizeof(double) ) ;
  aref = (double *)malloc ( nin*sizeof(double) ) ;
  /* get physical memory */
  {
    double tmp = 1.0/nin ;
    for ( int i=0; i<nin; ++i ) {
      a[i] = 0.0 ;
      b[i] = 1.0 + (i-nin/2)*tmp ;
    }
    tmp = 1.0/ntot ;
    for ( int j=0; j<nout; ++j ) {
      for ( int i=0; i<nin; ++i ) {
        int id = i + j*nin ;
	c[id] = 1.0 + (id - ntot/2)*tmp ;
      }
    }
  }
  /*--------------------------------------------------------------------
   *  fma0
   *------------------------------------------------------------------*/
  idm = 0 ;
#ifdef __USE_CPU_TIMER__
  elpst = get_cpu_time () ;
#endif
  for ( int itr=0; itr<10000; ++itr ) {
    fma0 ( a, b, c, nout, nin, itr, &idm) ; 
    idm += 1 ;
  }
#ifdef __USE_CPU_TIMER__
  elp[0] = get_cpu_time () ;
  elp[0] = elp[0] - elpst ;
  printf ("* fma0: original loop                     \n") ;
  printf ("CPU time (sec) =    %17.6f\n", elp[0] ) ;
#endif
  printf ("Num of iterations = %d    \n", idm ) ;
  for ( int i=0; i<nin; ++i ) aref[i] = a[i] ;
  /*--------------------------------------------------------------------
   *  fma4_nf 
   *------------------------------------------------------------------*/
  for ( int i=0; i<nin; ++i ) a[i] = 0.0 ;
  idm = 0 ;
#ifdef __USE_CPU_TIMER__
  elpst = get_cpu_time () ;
#endif
  for ( int itr=0; itr<10000; ++itr ) {
    fma4_nf ( a, b, c, nout, nin, itr, &idm) ; 
    idm += 1 ;
  }
#ifdef __USE_CPU_TIMER__
  elp[1] = get_cpu_time () ;
  elp[1] = elp[1] - elpst ;
  printf ("* fma4_nf: Unrolling expanding 4 times,   \n") ;
  printf ("           but no loop fusion             \n") ;
  printf ("CPU time (sec) =    %17.6f\n", elp[1] ) ;
#endif
  printf ("Num of iterations = %d    \n", idm ) ;
  for ( int i=0; i<nin; ++i ) a[i] = a[i] - aref[i] ;
  res = 0.0 ;
  for ( int i=0; i<nin; ++i ) res += a[i]*a[i] ;
  res = sqrt(res) ;
  printf ("Diff. on output b/w (fma4_nf) and (fma0)\n") ;
  printf ("%23.12e\n", res) ;
  /*-------------------------------------------------------------------
   *  fma4 
   *------------------------------------------------------------------*/
  for ( int i=0; i<nin; ++i ) a[i] = 0.0 ;
  idm = 0 ;
#ifdef __USE_CPU_TIMER__
  elpst = get_cpu_time () ;
#endif
  for ( int itr=0; itr<10000; ++itr ) {
    fma4 ( a, b, c, nout, nin, itr, &idm) ; 
    idm += 1 ;
  }
#ifdef __USE_CPU_TIMER__
  elp[2] = get_cpu_time () ;
  elp[2] = elp[2] - elpst ;
  printf ("* fma4: Unrolling expanding 4 times,      \n") ;
  printf ("        and loop fusion                   \n") ;
  printf ("CPU time (sec) =    %17.6f\n", elp[2] ) ;
#endif
  printf ("Num of iterations = %d    \n", idm ) ;
  for ( int i=0; i<nin; ++i ) a[i] = a[i] - aref[i] ;
  res = 0.0 ;
  for ( int i=0; i<nin; ++i ) res += a[i]*a[i] ;
  res = sqrt(res) ;
  printf ("Diff. on output b/w (fma4) and (fma0)\n") ;
  printf ("%23.12e\n", res) ;
  /*--------------------------------------------------------------------
   *  finalization 
   *------------------------------------------------------------------*/
  free ( a ) ; free ( b ); free ( aref ) ; free ( c ) ;
  return EXIT_SUCCESS ; 
}
/*----------------------------------------------------------------------
 *  fma0
 *--------------------------------------------------------------------*/
void fma0 ( double *a, double *b, double *c, 
           const int nout, const int nin, const int itr, int *idm ) {

  for ( int j=0; j<nout; ++j ) {
    for ( int i=0; i<nin; ++i ) {
      int id= i + j*nin ;
      a[i] += b[i]*c[id] ;
    }
  }

  *idm = itr ;
}
/*----------------------------------------------------------------------
 *  fma4_nf (loop unrolling expanding 4 times in outer loop, 
 *           but no loop fusion)
 *--------------------------------------------------------------------*/
void fma4_nf ( double *a, double *b, double *c, 
           const int nout, const int nin, const int itr, int *idm ) {

  int nres = nout - 4*(nout/4) ;

  /*--------------------------------------------------------------------
   * unrolling 
   *------------------------------------------------------------------*/
  for ( int j=0; j<nout-nres; j+=4 ) {

    // increase the number of statements in loop w.r.t. j
    for ( int i=0; i<nin; ++i ) {
      int id= i + j*nin ;
      a[i] += b[i]*c[id] ;
    }
    for ( int i=0; i<nin; ++i ) {
      int id= i + (j+1)*nin ;
      a[i] += b[i]*c[id] ;
    }
    for ( int i=0; i<nin; ++i ) {
      int id= i + (j+2)*nin ;
      a[i] += b[i]*c[id] ;
    }
    for ( int i=0; i<nin; ++i ) {
      int id= i + (j+3)*nin ;
      a[i] += b[i]*c[id] ;
    }

  }
  /*--------------------------------------------------------------------
   * residue 
   *------------------------------------------------------------------*/
  for ( int j=nout-nres; j<nout; ++j ) {
    for ( int i=0; i<nin; ++i ) {
      int id= i + j*nin ;
      a[i] += b[i]*c[id] ;
    }
  }

  *idm = itr ;
}
/*---------------------------------------------------------------------
 *  fma4 (loop unrolling with 4 unrolling factors in outer loop,
 *        and loop fusion, i.e., unroll and jam)
 *-------------------------------------------------------------------*/
void fma4 ( double *a, double *b, double *c, 
           const int nout, const int nin, const int itr, int *idm ) {

  int nres = nout - 4*(nout/4) ;

  /*--------------------------------------------------------------------
   * unrolling 
   *------------------------------------------------------------------*/
  for ( int j=0; j<nout-nres; j+=4 ) {

    // increase the number of statements in loop w.r.t. j
    for ( int i=0; i<nin; ++i ) {
      int id= i + j*nin ;
      a[i] += b[i]*c[id      ] ;
      a[i] += b[i]*c[id+  nin] ;
      a[i] += b[i]*c[id+2*nin] ;
      a[i] += b[i]*c[id+3*nin] ;
    }

  }
  /*--------------------------------------------------------------------
   * residue 
   *------------------------------------------------------------------*/
  for ( int j=nout-nres; j<nout; ++j ) {
    for ( int i=0; i<nin; ++i ) {
      int id= i + j*nin ;
      a[i] += b[i]*c[id] ;
    }
  }

  *idm = itr ;
}


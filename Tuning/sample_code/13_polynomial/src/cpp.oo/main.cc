// Copyright 2024 Research Organization for Information Science and Technology
/**********************************************************************
 * Some 'benchmark' of multiplication and addition
 * Authors:          Yukihiro Ota (yota@rist.or.jp)
 * Original Authors: Tatsunobu Kokubo
 * Last Update:      11 Oct. 2017
 * Remark:           This code is written according to 
 *                   the C++11 standard.
 *                   This is an object oriented code with a simple 
 *                   fixed-length vector template, vector_oo.h.
 * Reference:        HPCI Research Report, hp130038
 *                   "A Performance Analysis of Evaluating Polynomials
 *                   with Expression Templates in Supercomputer K"
 *                   http://www.hpci-office.jp/annex/resrep/
 **********************************************************************/
#include "vector_oo.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "mykernel.h"

/* Wall clock */
#if ! defined(NOT_USE_REALTIME)
double get_elp_time () {
  struct timespec tp ;
  clock_gettime ( CLOCK_REALTIME, &tp ) ;
  return  tp.tv_sec + (double)tp.tv_nsec*1.0e-9 ;
}
#else
double get_elp_time () {
  struct timespec tp ;
  clock_gettime ( CLOCK_MONOTONIC, &tp ) ;
  return  tp.tv_sec + (double)tp.tv_nsec*1.0e-9 ;
}
#endif

/* CPU time */
double get_cpu_time () {
  struct timespec tp ;
  clock_gettime ( CLOCK_PROCESS_CPUTIME_ID, &tp ) ;
  return  tp.tv_sec + (double)tp.tv_nsec*1.0e-9 ;
}

// ARRAY_SIZE is defined in mykernel.h

int main ( int argc, char* argv[] )
{
  const int nrep = 100000 ;
  const int ndim = ARRAY_SIZE ;

  double flop, mem ; 
  vector_oo<double,ndim> x ;
  vector_oo<double,ndim> c0 ,c1  ;

  double elp1, elp2, elp, flops ; 

  /* set coefficients */
  double drep = 1.0/nrep ;
  srand( (unsigned int)time(NULL) ) ;
  for( int i=0; i<ndim; ++i ) {
    double tmp1 = (double)rand()/RAND_MAX ;
    double tmp2 = (double)rand()/RAND_MAX ;
    c0[i]  = 2.0*(tmp1-0.5)*drep ;
    c1[i]  = 2.0*(tmp1-0.5)*drep/2.0 ;
  }
  printf("******************************************************\n") ;
  printf("Some benchmark of multiplication and addition\n") ;
  printf("Authors:      Yukihiro Ota (yota@rist.or.jp)\n") ;
  printf("              Tatsunobu Kokubo\n") ;
  printf("Last Update:  11 Oct 2017\n") ;
  printf("Description:  This code is written with C++11 standard.\n") ;
  printf("              This is an object oriented code with a   \n") ;
  printf("              fixed-length vector template, vector_oo.h\n") ;
  printf("[setup]\n") ;
  printf("Number of repetitions: %d\n",nrep) ;
  printf("Number of elements   : %d\n",ndim) ;
  printf("******************************************************\n") ;
  /*********************************************************************
   * This kernel is made according to an idea in 9th Degree Polynomial
   * (inside mod1a) of EuroBen (https://www.euroben.nl/index.php)
   ********************************************************************/
  /* 1 FMA                                                            */
  for ( int i=0; i<ndim; ++i ) x[i] = 0.0 ;
  flop = 2.0*ndim ;
  mem  = 3.0*8.0*ndim ; // setting < L2 cache is preferable 
  printf("[run] 1 FMA\n") ;
  printf("Giga FLOP              : %20.9f\n", flop*1.0e-9);
  printf("Memory (MB)            : %20.9f\n", mem*1.0e-6) ;
  printf("start ...\n") ;

  elp1 = get_elp_time() ;
  for( int irep=0; irep<nrep; ++irep ) {
    /* main kernel */
    poly1(x,c0,c1);
    x[0] += drep*irep ; /* prevent optimization by smart compiler */
  }
  elp2 = get_elp_time() ;

  elp = elp2 - elp1 ;
  flops = (1.0e-9*flop*nrep)/elp ; /* Giga flop/s */
  printf("Elapsed (sec.)         : %10.4f\n",elp) ;
  printf("Giga FLOPs             : %10.4f\n",flops) ;
  printf("x[0] %13.6f\n",x[0]) ;

  /* 4 FMA                                                            */
  for ( int i=0; i<ndim; ++i ) x[i] = 0.0 ;
  flop = 8.0*ndim ;
  mem  = 3.0*8.0*ndim ; // setting < L2 cache is preferable
  printf("[run] 4 FMA\n") ;
  printf("Giga FLOP              : %20.9f\n", flop*1.0e-9);
  printf("Memory (MB)            : %20.9f\n", mem*1.0e-6) ;
  printf("start ...\n") ;

  elp1 = get_elp_time() ;
  for( int irep=0; irep<nrep; ++irep ) {
    /* main kernel */
    poly4(x,c0,c1);
    x[0] += drep*irep ; /* prevent optimization by smart compiler */
  }
  elp2 = get_elp_time() ;

  elp = elp2 - elp1 ;
  flops = (1.0e-9*flop*nrep)/elp ; /* Giga flop/s */
  printf("Elapsed (sec.)         : %10.4f\n",elp) ;
  printf("Giga FLOPs             : %10.4f\n",flops) ;
  printf("x[0] %13.6f\n",x[0]) ;

  /* 8 FMA                                                            */
  for ( int i=0; i<ndim; ++i ) x[i] = 0.0 ;
  flop = 16.0*ndim ;
  mem  = 3.0*8.0*ndim ; // setting < L2 cache is preferable
  printf("[run] 8 FMA\n") ;
  printf("Giga FLOP              : %20.9f\n", flop*1.0e-9);
  printf("Memory (MB)            : %20.9f\n", mem*1.0e-6) ;
  printf("start ...\n") ;

  elp1 = get_elp_time() ;
  for( int irep=0; irep<nrep; ++irep ) {
    /* main kernel */
    poly8(x,c0,c1);
    x[0] += drep*irep ; /* prevent optimization by smart compiler */
  }
  elp2 = get_elp_time() ;

  elp = elp2 - elp1 ;
  flops = (1.0e-9*flop*nrep)/elp ; /* Giga flop/s */
  printf("Elapsed (sec.)         : %10.4f\n",elp) ;
  printf("Giga FLOPs             : %10.4f\n",flops) ;
  printf("x[0] %13.6f\n",x[0]) ;

  /* 16 FMA                                                           */
  for ( int i=0; i<ndim; i++ ) x[i] = 0.0 ;
  flop = 32.0*ndim ;
  mem  = 3.0*8.0*ndim ; // setting < L2 cache is preferable
  printf("[run] 16 FMA\n") ;
  printf("Giga FLOP              : %20.9f\n", flop*1.0e-9);
  printf("Memory (MB)            : %20.9f\n", mem*1.0e-6) ;
  printf("start ...\n") ;

  elp1 = get_elp_time() ;
  for( int irep=0; irep<nrep; ++irep ) {
    /* main kernel */
    poly16(x,c0,c1);
    x[0] += drep*irep ; /* prevent optimization by smart compiler */
  }
  elp2 = get_elp_time() ;

  elp = elp2 - elp1 ;
  flops = (1.0e-9*flop*nrep)/elp ; /* Giga flop/s */
  printf("Elapsed (sec.)         : %10.4f\n",elp) ;
  printf("Giga FLOPs             : %10.4f\n",flops) ;
  printf("x[0] %13.6f\n",x[0]) ;

  return EXIT_SUCCESS ;
}

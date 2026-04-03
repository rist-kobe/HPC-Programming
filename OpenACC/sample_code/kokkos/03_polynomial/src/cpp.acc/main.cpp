// Copyright 2025 Research Organization for Information Science and Technology
/**********************************************************************
 * Some 'benchmark' of multiplication and addition
 * Authors:          Yukihiro Ota (yota@rist.or.jp)
 * Original Authors: Tatsunobu Kokubo
 * Last Update:      11 Oct. 2017
 * Remark:           This code is written according to c++11 standard.
 * Reference:        HPCI Research Report, hp130038
 *                   "A Performance Analysis of Evaluating Polynomials
 *                   with Expression Templates in Supercomputer K"
 *                   http://www.hpci-office.jp/annex/resrep/
 **********************************************************************/
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "mykernel.h"
#include <openacc.h>

/* Wall clock */
double get_elp_time () {
  struct timespec tp ;
  clock_gettime ( CLOCK_REALTIME, &tp ) ;
  return  tp.tv_sec + (double)tp.tv_nsec*1.0e-9 ;
}

/* CPU time */
double get_cpu_time () {
  struct timespec tp ;
  clock_gettime ( CLOCK_PROCESS_CPUTIME_ID, &tp ) ;
  return  tp.tv_sec + (double)tp.tv_nsec*1.0e-9 ;
}

#define ARRAY_SIZE 10000

int main ( int argc, char* argv[] )
{
  const int nrep = 100000 ;
  //const int ndim = ARRAY_SIZE ;
  int ndim, icon;

  double flop, mem ; 
  double *x, *c0, *c1;
  double elp1, elp2, elp, flops ; 

  if ( argc != 2 ) {
     printf("[usage] run.x (arg1)\n");
     printf("        (arg1): array size (int); default: 10000\n");
     return EXIT_SUCCESS;
  }

  ndim = atoi(argv[1]);
  if ( ndim < 0 ) {
     ndim = ARRAY_SIZE;
  }

  x  = (double *)malloc( ndim*sizeof(double) );
  c0 = (double *)malloc( ndim*sizeof(double) );
  c1 = (double *)malloc( ndim*sizeof(double) );

  /* set coefficients */
  double drep = 1.0/nrep ;
  //srand( (unsigned int)time(NULL) ) ;
  srand( 37983 ) ;
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
  printf("Last Update:  Mar 17 2025\n") ;
  printf("Description:  This code is written by C++11 standard.\n") ;
  printf("              OpenACC\n") ;
  printf("[setup]\n") ;
  printf("Number of repetitions: %d\n",nrep) ;
  printf("Number of elements   : %d\n",ndim) ;
  {
  acc_device_t dev_type = acc_get_device_type();
  int ng = acc_get_num_devices(dev_type);
  printf("Number of devices    : %d\n",ng) ;
  }
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

  #pragma acc data copyin(c0[0:ndim],c1[0:ndim]) copy(x[0:ndim])
  {
  elp1 = get_elp_time() ;
  for( int irep=0; irep<nrep; ++irep ) {
    poly1(x,c0,c1,ndim);
    icon += dummy(x,irep,drep); /* prevent optimization by smart compiler */
  }
  elp2 = get_elp_time() ;
  } // acc data

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

  #pragma acc data copyin(c0[0:ndim],c1[0:ndim]) copy(x[0:ndim])
  {
  elp1 = get_elp_time() ;
  for( int irep=0; irep<nrep; ++irep ) {
    poly4(x,c0,c1,ndim);
    icon += dummy(x,irep,drep); /* prevent optimization by smart compiler */
  }
  elp2 = get_elp_time() ;
  } // acc data

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

  #pragma acc data copyin(c0[0:ndim],c1[0:ndim]) copy(x[0:ndim])
  {
  elp1 = get_elp_time() ;
  for( int irep=0; irep<nrep; ++irep ) {
    poly8(x,c0,c1,ndim);
    icon += dummy(x,irep,drep); /* prevent optimization by smart compiler */
  }
  elp2 = get_elp_time() ;
  } // acc data

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

  #pragma acc data copyin(c0[0:ndim],c1[0:ndim]) copy(x[0:ndim])
  {
  elp1 = get_elp_time() ;
  for( int irep=0; irep<nrep; ++irep ) {
    poly16(x,c0,c1,ndim);
    icon += dummy(x,irep,drep); /* prevent optimization by smart compiler */
  }
  elp2 = get_elp_time() ;
  } // acc data

  elp = elp2 - elp1 ;
  flops = (1.0e-9*flop*nrep)/elp ; /* Giga flop/s */
  printf("Elapsed (sec.)         : %10.4f\n",elp) ;
  printf("Giga FLOPs             : %10.4f\n",flops) ;
  printf("x[0] %13.6f\n",x[0]) ;

  /* 32 FMA                                                           */
  for ( int i=0; i<ndim; i++ ) x[i] = 0.0 ;
  flop = 64.0*ndim ;
  mem  = 3.0*8.0*ndim ; // setting < L2 cache is preferable
  printf("[run] 32 FMA\n") ;
  printf("Giga FLOP              : %20.9f\n", flop*1.0e-9);
  printf("Memory (MB)            : %20.9f\n", mem*1.0e-6) ;
  printf("start ...\n") ;

  #pragma acc data copyin(c0[0:ndim],c1[0:ndim]) copy(x[0:ndim])
  {
  elp1 = get_elp_time() ;
  for( int irep=0; irep<nrep; ++irep ) {
    poly32(x,c0,c1,ndim);
    icon += dummy(x,irep,drep); /* prevent optimization by smart compiler */
  }
  elp2 = get_elp_time() ;
  } // acc data

  elp = elp2 - elp1 ;
  flops = (1.0e-9*flop*nrep)/elp ; /* Giga flop/s */
  printf("Elapsed (sec.)         : %10.4f\n",elp) ;
  printf("Giga FLOPs             : %10.4f\n",flops) ;
  printf("x[0] %13.6f\n",x[0]) ;

  return EXIT_SUCCESS ;
}

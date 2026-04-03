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
#include <Kokkos_Core.hpp>
#include "mykernel.hpp"
#include "types.hpp"
#include <typeinfo>

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
  int ndim, icon, ideg;

  double flop, mem ; 
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

  Kokkos::initialize(argc, argv);
  {
  MyKernel kernel(ndim);

  /* set coefficients */
  FPtype drep = 1.0/nrep ;
  unsigned int iseed = 37983;
  kernel.create_data(iseed, drep);

  printf("******************************************************\n") ;
  printf("Some benchmark of multiplication and addition\n") ;
  printf("Authors:      Yukihiro Ota (yota@rist.or.jp)\n") ;
  printf("              Tatsunobu Kokubo\n") ;
  printf("Last Update:  11 Oct 2017\n") ;
  printf("Description:  This code is written by C++14 standard.\n") ;
  printf("[setup]\n") ;
  printf("Number of repetitions: %d\n",nrep) ;
  printf("Number of elements   : %d\n",ndim) ;
  {
  int ng;
  CHECK(cudaGetDeviceCount(&ng));
  printf("Number of devices    : %d\n", ng);
  printf("Kokkos execution space: %s\n", typeid(ExecSpace).name());
  }
  printf("******************************************************\n") ;
  /*********************************************************************
   * This kernel is made according to an idea in 9th Degree Polynomial
   * (inside mod1a) of EuroBen (https://www.euroben.nl/index.php)
   ********************************************************************/
  /* 1 FMA                                                            */
  for ( int i=0; i<ndim; ++i ) kernel.h_x(i) = 0.0;
  Kokkos::deep_copy( kernel.d_x, kernel.h_x ); // implicit barrier
  flop = 2.0*ndim ;
  mem  = 3.0*8.0*ndim ; // setting < L2 cache is preferable 
  printf("[run] 1 FMA\n") ;
  printf("Giga FLOP              : %20.9f\n", flop*1.0e-9);
  printf("Memory (MB)            : %20.9f\n", mem*1.0e-6) ;
  printf("start ...\n") ;

  elp1 = get_elp_time() ;
  icon = 0;
  ideg = 1;
  for( int irep=0; irep<nrep; ++irep ) {
    kernel.poly(ideg);
    icon += kernel.dummy(irep,drep); /* prevent optimization by smart compiler */
    Kokkos::fence(); // explicit barrier
  }
  elp2 = get_elp_time() ;
  Kokkos::deep_copy( kernel.h_x, kernel.d_x ); // implicit barrier

  elp = elp2 - elp1 ;
  flops = (1.0e-9*flop*nrep)/elp ; /* Giga flop/s */
  printf("Elapsed (sec.)         : %10.4f\n",elp) ;
  printf("Giga FLOPs             : %10.4f\n",flops) ;
  printf("x[0] %13.6f\n",kernel.h_x(0)) ;

  /* 4 FMA                                                            */
  for ( int i=0; i<ndim; ++i ) kernel.h_x(i) = 0.0;
  Kokkos::deep_copy( kernel.d_x, kernel.h_x ); // implicit barrier
  flop = 8.0*ndim ;
  mem  = 3.0*8.0*ndim ; // setting < L2 cache is preferable
  printf("[run] 4 FMA\n") ;
  printf("Giga FLOP              : %20.9f\n", flop*1.0e-9);
  printf("Memory (MB)            : %20.9f\n", mem*1.0e-6) ;
  printf("start ...\n") ;

  elp1 = get_elp_time() ;
  icon = 0;
  ideg = 4;
  for( int irep=0; irep<nrep; ++irep ) {
    kernel.poly(ideg);
    icon += kernel.dummy(irep,drep); /* prevent optimization by smart compiler */
    Kokkos::fence(); // explicit barrier
  }
  elp2 = get_elp_time() ;
  Kokkos::deep_copy( kernel.h_x, kernel.d_x ); // implicit barrier

  elp = elp2 - elp1 ;
  flops = (1.0e-9*flop*nrep)/elp ; /* Giga flop/s */
  printf("Elapsed (sec.)         : %10.4f\n",elp) ;
  printf("Giga FLOPs             : %10.4f\n",flops) ;
  printf("x[0] %13.6f\n",kernel.h_x(0)) ;

  /* 8 FMA                                                            */
  for ( int i=0; i<ndim; ++i ) kernel.h_x(i) = 0.0;
  Kokkos::deep_copy( kernel.d_x, kernel.h_x ); // implicit barrier
  flop = 16.0*ndim ;
  mem  = 3.0*8.0*ndim ; // setting < L2 cache is preferable
  printf("[run] 8 FMA\n") ;
  printf("Giga FLOP              : %20.9f\n", flop*1.0e-9);
  printf("Memory (MB)            : %20.9f\n", mem*1.0e-6) ;
  printf("start ...\n") ;

  elp1 = get_elp_time() ;
  icon = 0;
  ideg = 8;
  for( int irep=0; irep<nrep; ++irep ) {
    kernel.poly(ideg);
    icon += kernel.dummy(irep,drep); /* prevent optimization by smart compiler */
    Kokkos::fence(); // explicit barrier
  }
  elp2 = get_elp_time() ;
  Kokkos::deep_copy( kernel.h_x, kernel.d_x ); // implicit barrier

  elp = elp2 - elp1 ;
  flops = (1.0e-9*flop*nrep)/elp ; /* Giga flop/s */
  printf("Elapsed (sec.)         : %10.4f\n",elp) ;
  printf("Giga FLOPs             : %10.4f\n",flops) ;
  printf("x[0] %13.6f\n",kernel.h_x(0)) ;

  /* 16 FMA                                                           */
  for ( int i=0; i<ndim; ++i ) kernel.h_x(i) = 0.0;
  Kokkos::deep_copy( kernel.d_x, kernel.h_x ); // implicit barrier
  flop = 32.0*ndim ;
  mem  = 3.0*8.0*ndim ; // setting < L2 cache is preferable
  printf("[run] 16 FMA\n") ;
  printf("Giga FLOP              : %20.9f\n", flop*1.0e-9);
  printf("Memory (MB)            : %20.9f\n", mem*1.0e-6) ;
  printf("start ...\n") ;

  elp1 = get_elp_time() ;
  icon = 0;
  ideg = 16;
  for( int irep=0; irep<nrep; ++irep ) {
    kernel.poly(ideg);
    icon += kernel.dummy(irep,drep); /* prevent optimization by smart compiler */
    Kokkos::fence(); // explicit barrier
  }
  elp2 = get_elp_time() ;
  Kokkos::deep_copy( kernel.h_x, kernel.d_x ); // implicit barrier

  elp = elp2 - elp1 ;
  flops = (1.0e-9*flop*nrep)/elp ; /* Giga flop/s */
  printf("Elapsed (sec.)         : %10.4f\n",elp) ;
  printf("Giga FLOPs             : %10.4f\n",flops) ;
  printf("x[0] %13.6f\n",kernel.h_x(0)) ;

  /* 32 FMA                                                           */
  for ( int i=0; i<ndim; ++i ) kernel.h_x(i) = 0.0;
  Kokkos::deep_copy( kernel.d_x, kernel.h_x ); // implicit barrier
  flop = 64.0*ndim ;
  mem  = 3.0*8.0*ndim ; // setting < L2 cache is preferable
  printf("[run] 32 FMA\n") ;
  printf("Giga FLOP              : %20.9f\n", flop*1.0e-9);
  printf("Memory (MB)            : %20.9f\n", mem*1.0e-6) ;
  printf("start ...\n") ;

  elp1 = get_elp_time() ;
  icon = 0;
  ideg = 32;
  for( int irep=0; irep<nrep; ++irep ) {
    kernel.poly(ideg);
    icon += kernel.dummy(irep,drep); /* prevent optimization by smart compiler */
    Kokkos::fence(); // explicit barrier
  }
  elp2 = get_elp_time() ;
  Kokkos::deep_copy( kernel.h_x, kernel.d_x ); // implicit barrier

  elp = elp2 - elp1 ;
  flops = (1.0e-9*flop*nrep)/elp ; /* Giga flop/s */
  printf("Elapsed (sec.)         : %10.4f\n",elp) ;
  printf("Giga FLOPs             : %10.4f\n",flops) ;
  printf("x[0] %13.6f\n",kernel.h_x(0)) ;
  } 
  Kokkos::finalize();

  return EXIT_SUCCESS ;
}

// Copyright 2025 Research Organization for Information Science and Technology
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <Kokkos_Core.hpp>
#include "mykernel_kk.h"
#include "timer.h"
#include "type.h"

#define STRIDE_DP 32
#define NITR 100000
#define SCALEFAC 1.0e-1

int main (int argc, char **argv)
{
  int inum, itrue_rate; 
  int calctype;
  double rcut2, t_rate;
  double elp[3]; 
  double ghalf1, f1, ghalf2, f2;

  if ( argc != 3 ) {
    fprintf (stdout, "[usage] run.x (arg1) (arg2)\n");
    fprintf (stdout, "  (arg1): array size (integer)\n");
    fprintf (stdout, "  (arg2): true rate of if statement (integer)\n");
    fprintf (stdout, "          -1 means default (arg2=10).\n");
    fprintf (stdout, "          0 means no true case.\n");
    fprintf (stdout, "          Larger means more true.\n");
    return EXIT_SUCCESS;
  }

  inum = atoi(argv[1]);
  if ( inum <= 0 ) {
     inum = 200;
  }

  itrue_rate = atoi(argv[2]);
  if ( itrue_rate < 0 ) {
     itrue_rate = 10;
  }
  rcut2 = 0.5e-1 * itrue_rate * inum * SCALEFAC;
  rcut2 = rcut2 * rcut2;

  if ( inum > MAXINUM ) {
     fprintf(stdout, "Error: inum must be smaller than %d\n", MAXINUM);
     return EXIT_FAILURE;
  }

  Kokkos::initialize(argc, argv);
  {

  // Data preparation
  MyKernel_kk kernel;

  kernel.init(inum, rcut2, STRIDE_DP, SCALEFAC); // implicit barrier

  double f;

  // Check true rate in if statement
  kernel.count_true(t_rate); 

  // Kerel 1
  for ( int i = 0; i < inum; ++i ) {
     kernel.h_g( i ) = 0.0;
  }
  Kokkos::deep_copy( kernel.d_g, kernel.h_g ); // implicit barrier

  f = 0.0; 

  calctype = 1;
  kernel.do_kernel(calctype, NITR, f, elp[0]); 
  Kokkos::deep_copy( kernel.h_g, kernel.d_g ); // implicit barrier
  ghalf1 = kernel.h_g( inum/2 );
  f1 = f;

  // Kerel 2
  for ( int i = 0; i < inum; ++i ) {
     kernel.h_g( i ) = 0.0;
  }
  Kokkos::deep_copy( kernel.d_g, kernel.h_g ); // implicit barrier

  f = 0.0; 

  calctype = 2;
  kernel.do_kernel(calctype, NITR, f, elp[1]); 
  Kokkos::deep_copy( kernel.h_g, kernel.d_g ); // implicit barrier
  ghalf2 = kernel.h_g( inum/2 );
  f2 = f;

  fprintf(stdout,"Array size : %d\n", inum);
  fprintf(stdout,"True rate  : %11.2f\n", t_rate);
  fprintf(stdout,"Elapsed time (sec.)\n");
  fprintf(stdout,"  Kernel 1    :  %12.3f\n", elp[0]);
  fprintf(stdout,"  Kernel 2    :  %12.3f\n", elp[1]);
  fprintf(stdout,"Dummy output\n");
  fprintf(stdout,"  Kernel 1    :  %16.5f %16.5f\n", ghalf1, f1);
  fprintf(stdout,"  Kernel 2    :  %16.5f %16.5f\n", ghalf2, f2);
  }
  Kokkos::finalize();

  return EXIT_SUCCESS;
}

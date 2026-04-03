// Copyright 2025 Research Organization for Information Science and Technology 
// matvec: y <- A*x
#include <cstdio>
#include <cstdlib>
#include "mykernel_kk.hpp"
#include "timer.hpp"
#include "type.hpp"
#include <typeinfo>

int main (int argc, char **argv) 
{
   int N, NEXP, M, MEXP;
   int nitr;

   const double CONV2GIGA = 1.0e-9;
   double elp0, time;
   double fp_dp, mem_dp;
   double N_dp, M_dp;
   double chk0, chk1, chk2;
   double a;

   if ( argc != 4 ) {
     fprintf(stdout,"[usage] run.x (arg1) (arg2) (arg3)\n");
     fprintf(stdout,"  y <- A*x \n");
     fprintf(stdout,"  (arg1): N; Number specifing row size, with 2^N; defaut: N=10\n");
     fprintf(stdout,"  (arg2): M; Number specifing column size, with 2^M; defaut: M=10\n");
     fprintf(stdout,"  (arg3): nitr; Number of iterations (int); default: 5000\n");
     return EXIT_SUCCESS;
   }

   NEXP = atoi(argv[1]); 
   MEXP = atoi(argv[2]); 
   nitr = atoi(argv[3]); 

   if ( NEXP < 0 ) {
      N = 1 << 10;
   } else {
      N = 1 << NEXP; 
   }

   if ( MEXP < 0 ) {
      M = 1 << 10;
   } else {
      M = 1 << MEXP; 
   }

   if ( nitr < 0 ) {
      nitr = 5000;
   }

   N_dp = static_cast<double>(N);
   M_dp = static_cast<double>(M);
   
   fp_dp  = 2.0*CONV2GIGA*N_dp*M_dp*nitr;
   mem_dp = (8.0*N_dp + 8.0*N_dp*M_dp + 8.0*M_dp)*CONV2GIGA*nitr; 

   a = 1.0e-3 / nitr;

   fprintf(stdout, "#===============================\n");
   fprintf(stdout, "# Setting\n");
   fprintf(stdout, "#===============================\n");
   fprintf(stdout, "Row size                        : %11d\n", N);
   fprintf(stdout, "Column size                     : %11d\n", M);
   fprintf(stdout, "Num of iterationsn              : %11d\n", nitr);

   fprintf(stdout, "#===============================\n");
   fprintf(stdout, "# Measurements\n");
   fprintf(stdout, "#===============================\n");

   Kokkos::initialize(argc, argv);
   {
#if 1
   // Header information (not fully implemented)
   { 
#if defined(USE_SERIAL)
   fprintf(stdout,"Serial execution on host\n");
#elif defined(USE_OPENMP)
   int nt = omp_get_max_threads();
   fprintf(stdout,"OpenMP parallel on host\n");
   fprintf(stdout,"Number of threads  :   %d\n", nt);
#elif defined(USE_OPENMPTARGET)
   int ng = omp_get_num_devices();
   fprintf(stdout,"OpenMP Target on device\n");
   fprintf(stdout,"Number of devices  :   %d\n", ng);
#else // default: CUDA 
   int ng;
   CHECK(cudaGetDeviceCount(&ng));
   fprintf(stdout,"CUDA on device\n");
   fprintf(stdout,"Number of devices  :   %d\n", ng);
#endif   
   }
#endif
   fprintf(stdout, "Kokkos execution space: %s\n", typeid(ExecSpace).name());

   MyKernel_kk kernel(N, a, N, M, M);

   kernel.init_mv(); // implicit barrier
   
   elp0 = get_elp_time();

   // Measure kernel

   int icon = 0;
   for ( int itr = 0; itr < nitr; ++itr ) { 
      kernel.mvp_simple_kk ();
      icon += kernel.dummy_kk (itr, N); 
      Kokkos::fence();  // explicit barrier
   } 

   time = get_elp_time() - elp0;

   // Deep copy device view to host view
   Kokkos::deep_copy( kernel.h_y, kernel.d_y ); // implicit barrier

   chk0 = kernel.h_y( 0 ) - (0.5 * (M-1));
   chk1 = kernel.h_y( N/2 ) - (0.5 * (M-1));
   chk2 = kernel.h_y( N-1 ) - (0.5 * (M-1));
   fprintf(stdout, "%-15s %10s %10s %10s %15s %15s %15s\n",
     "type","elp_sec","GFLOPs","GiBs","chk0","chk1","chk2");
   fprintf(stdout, "%-15s %10.3f %10.3f %10.3f %15.6E %15.6E %15.6E\n",
     "kokkos",time,fp_dp/time,mem_dp/time,chk0,chk1,chk2);
   }
   Kokkos::finalize();

   return EXIT_SUCCESS;
}

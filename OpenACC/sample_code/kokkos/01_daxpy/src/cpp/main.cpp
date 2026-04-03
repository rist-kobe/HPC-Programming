// Copyright 2025 Research Organization for Information Science and Technology 
// daxpy: y <- y + a*x
#include <cstdio>
#include <cstdlib>
#include "mykernel_kk.hpp"
#include "timer.hpp"
#include "type.hpp"
#include <typeinfo>

int main (int argc, char **argv) 
{
   int N, NEXP;
   int nitr;

   const double CONV2GIGA = 1.0e-9;
   double elp0, time;
   double fp_dp, mem_dp;
   double N_dp;
   double chk0, chk1, chk2;
   double a;

   if ( argc != 3 ) {
     fprintf(stdout,"[usage] run.x (arg1) (arg2)\n");
     fprintf(stdout,"  y <- y+a*x \n");
     fprintf(stdout,"  (arg1): N: vector size, with 2^N; defaut: N=22\n");
     fprintf(stdout,"  (arg2): nitr; Number of iterations (int); default: 10000\n");
     return EXIT_SUCCESS;
   }

   NEXP = atoi(argv[1]); 
   nitr = atoi(argv[2]); 

   if ( NEXP < 0 ) {
      N = 1 << 22;
   } else {
      N = 1 << NEXP; 
   }

   if ( nitr < 0 ) {
      nitr = 10000;
   }

   N_dp = static_cast<double>(N);
   
   fp_dp  = 2.0*CONV2GIGA*N_dp*nitr;
   mem_dp = (8.0*N_dp + 8.0*N_dp + 8.0*N_dp)*CONV2GIGA*nitr; 

   a = 1.0e-3 / nitr;

   fprintf(stdout, "#===============================\n");
   fprintf(stdout, "# Setting\n");
   fprintf(stdout, "#===============================\n");
   fprintf(stdout, "Vector size                     : %11d\n", N);
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

   MyKernel_kk kernel(N, a, N, N, N);

   kernel.init_daxpy(); // implicit barrier
   
#if 0   
   Kokkos::Timer timer;
#else
   elp0 = get_elp_time();
#endif

   // Measure kernel

   int icon = 0;
   for ( int itr = 0; itr < nitr; ++itr ) { 
      kernel.daxpy_kk ();
      icon += kernel.dummy_kk (itr, N); 
      Kokkos::fence();  // explicit barrier
   } 

#if 0
   time = timer.seconds();
#else
   time = get_elp_time() - elp0;
#endif

   // Deep copy device view to host view
   Kokkos::deep_copy( kernel.h_y, kernel.d_y ); // implicit barrier

   chk0 = kernel.h_y( 0 ) - (a * kernel.h_x( 0 ) * static_cast<double>(nitr));
   chk1 = kernel.h_y( N/2 ) - (a * kernel.h_x( N/2 ) * static_cast<double>(nitr));
   chk2 = kernel.h_y( N-1 ) - (a * kernel.h_x( N-1 ) * static_cast<double>(nitr));
   fprintf(stdout, "%-15s %10s %10s %10s %15s %15s %15s\n",
     "type","elp_sec","GFLOPs","GiBs","chk0","chk1","chk2");
   fprintf(stdout, "%-15s %10.3f %10.3f %10.3f %15.6E %15.6E %15.6E\n",
     "kokkos",time,fp_dp/time,mem_dp/time,chk0,chk1,chk2);
   }
   Kokkos::finalize();

   return EXIT_SUCCESS;
}

// Copyright 2025 Research Organization for Information Science and Technology 
// Note:
// This code contains many "#if defined" macros to switch kinds of implementation.
// -DUSE_MDRANGE: matmat: C <- A*B with MDRangePolicy
// -DUSE_HIER   : matmat: C <- A*B with MDRangePolicy and hierarchical parallelism
// nothing      : matmat: C <- A*B without MDRangePolicy

#include <cstdio>
#include <cstdlib>
#include "mykernel_kk.hpp"
#include "timer.hpp"
#include "type.hpp"
#include <typeinfo>

int main (int argc, char **argv) 
{
   int N, NEXP, M, MEXP, L, LEXP;
   int nitr;

   const double CONV2GIGA = 1.0e-9;
   double elp0, time;
   double fp_dp, mem_dp;
   double N_dp, M_dp, L_dp;
   double chk0, chk1, chk2;
   double a;

   if ( argc != 5 ) {
     fprintf(stdout,"[usage] run.x (arg1) (arg2) (arg3) (arg4)\n");
#if defined(USE_MDRRANGE)
     fprintf(stdout,"   C = A * B, C: NxM, A=NxL, B=LxM, with MDRangPolicy.\n");
#elif defined(USE_HIER)
     fprintf(stdout,"   C = A * B, C: NxM, A=NxL, B=LxM, with MDRangPolicy and hierarchical parallelism.\n");
#else
     fprintf(stdout,"   C = A * B, C: NxM, A=NxL, B=LxM, without MDRangPolicy.\n");
#endif
     fprintf(stdout,"  (arg1): N: matrix size, with 2^N; defaut: N=10\n");
     fprintf(stdout,"  (arg2): M; matrix size, with 2^M; defaut: M=10\n");
     fprintf(stdout,"  (arg3): L; matrix size, with 2^L; defaut: L=10\n");
     fprintf(stdout,"  (arg4): nitr; Number of iterations (int); default: 5000\n");
     return EXIT_SUCCESS;
   }

   NEXP = atoi(argv[1]); 
   MEXP = atoi(argv[2]); 
   LEXP = atoi(argv[3]); 
   nitr = atoi(argv[4]); 

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

   if ( LEXP < 0 ) {
      L = 1 << 10;
   } else {
      L = 1 << LEXP; 
   }

   if ( nitr < 0 ) {
      nitr = 5000;
   }

   N_dp = static_cast<double>(N);
   M_dp = static_cast<double>(M);
   L_dp = static_cast<double>(L);
   
   fp_dp  = 2.0*CONV2GIGA*N_dp*M_dp*L_dp*nitr;
   mem_dp = (8.0*N_dp*M_dp + 8.0*N_dp*L_dp + 8.0*M_dp*L_dp)*CONV2GIGA*nitr; 

   a = 1.0e-3 / nitr;

   fprintf(stdout, "#===============================\n");
   fprintf(stdout, "# Setting\n");
   fprintf(stdout, "#===============================\n");
#if defined(USE_MDRRANGE)
   fprintf(stdout, " C = A * B, C: NxM, A=NxL, B=LxM, with MDRangPolicy.\n");
#elif defined(USE_HIER)
   fprintf(stdout, " C = A * B, C: NxM, A=NxL, B=LxM, with MDRangPolicy and hierarchical parallelism.\n");
#else
   fprintf(stdout, " C = A * B, C: NxM, A=NxL, B=LxM, without MDRangPolicy.\n");
#endif
   fprintf(stdout, "Matrix size (A:row, C:row)      : %11d\n", N);
   fprintf(stdout, "Matrix size (B:column, C:column): %11d\n", M);
   fprintf(stdout, "Matrix size (A:column, B:row)   : %11d\n", L);
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

   MyKernel_kk kernel(N, a, N, M, L);

#if defined(USE_MDRRANGE)
   kernel.init_mm(); // implicit barrier
#elif defined(USE_HIER)
   kernel.init_mm(); // implicit barrier
#else
   kernel.init_mmnomd(); // implicit barrier
#endif
   
   // Timer products.
   elp0 = get_elp_time();

   // Measure kernel

   int icon = 0;
   for ( int itr = 0; itr < nitr; ++itr ) { 
#if defined(USE_MDRRANGE)
      kernel.mmp_simple_kk ();
#elif defined(USE_HIER)
      kernel.mmp_hier_kk ();
#else
      kernel.mmpnomd_simple_kk ();
#endif
      icon += kernel.dummy_kk (itr, N); 
      Kokkos::fence();  // explicit barrier
   } 

   // Calculate time.
   time = get_elp_time() - elp0;

#if defined(USE_MDRRANGE)
   // Deep copy device view to host view
   Kokkos::deep_copy( kernel.h_C, kernel.d_C ); // implicit barrier

   chk0 = kernel.h_C( 0, 0 ) - 0.5*(L-1);
   chk1 = kernel.h_C( N/2, M/2 ) - 0.5*(L-1);
   chk2 = kernel.h_C( N-1, M-1 ) - 0.5*(L-1);
#elif defined(USE_HIER)
   // Deep copy device view to host view
   Kokkos::deep_copy( kernel.h_C, kernel.d_C ); // implicit barrier

   chk0 = kernel.h_C( 0, 0 ) - 0.5*(L-1);
   chk1 = kernel.h_C( N/2, M/2 ) - 0.5*(L-1);
   chk2 = kernel.h_C( N-1, M-1 ) - 0.5*(L-1);
#else
   // Deep copy device view to host view
   Kokkos::deep_copy( kernel.h_cc, kernel.d_cc ); // implicit barrier

   chk0 = kernel.h_cc( 0 ) - 0.5*(L-1);
   chk1 = kernel.h_cc( (N/2)*M + M/2 ) - 0.5*(L-1);
   chk2 = kernel.h_cc( (N-1)*M + M-1 ) - 0.5*(L-1);
#endif

   fprintf(stdout, "%-15s %10s %10s %10s %15s %15s %15s\n",
     "type","elp_sec","GFLOPs","GiBs","chk0","chk1","chk2");
   fprintf(stdout, "%-15s %10.3f %10.3f %10.3f %15.6E %15.6E %15.6E\n",
     "kokkos",time,fp_dp/time,mem_dp/time,chk0,chk1,chk2);
   }
   Kokkos::finalize();

   return EXIT_SUCCESS;
}

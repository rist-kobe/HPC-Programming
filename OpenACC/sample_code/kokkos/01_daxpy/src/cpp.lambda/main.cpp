// Copyright 2025 Research Organization for Information Science and Technology 
// daxpy: y <- y + a*x
// https://kokkos.org/kokkos-core-wiki/usecases/Kokkos-Fortran-Interoperability.html
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <Kokkos_Core.hpp>
#include <typeinfo>

// Wall clock 
double get_elp_time () {
  struct timespec tp ;
  clock_gettime ( CLOCK_REALTIME, &tp ) ;
  return  tp.tv_sec + (double)tp.tv_nsec*1.0e-9 ;
}

// CPU time 
double get_cpu_time () {
  struct timespec tp ;
  clock_gettime ( CLOCK_PROCESS_CPUTIME_ID, &tp ) ;
  return  tp.tv_sec + (double)tp.tv_nsec*1.0e-9 ;
}

#if ! defined(KOKKOS_ENABLE_CUDA)
#error "Default setting is to use CUDA wit Kokkos_ENABLE_CUDA=ON"
#endif

#if defined(USE_OPENMP) || defined(USE_OPENMPTARGET)
#include "omp.h"
#else // default: CUDA
#define CHECK(call)                                                                 \
{                                                                                   \
   cudaError_t error = call;                                                        \
   if ( error != cudaSuccess )                                                      \
   {                                                                                \
      fprintf(stderr,"Error: %s:%d ", __FILE__, __LINE__ );                         \
      fprintf(stderr,"returned %d -> %s\n", (int)error, cudaGetErrorString(error)); \
      exit(EXIT_FAILURE);                                                           \
   }                                                                                \
}
#endif

int main (int argc, char **argv) 
{
   int N, NEXP;
   int nitr;
   const double CONV2GIGA = 1.0e-9;

   if ( argc != 3 ) {
     fprintf(stdout,"[usage] run.x (arg1) (arg2)\n");
     fprintf(stdout,"  (arg1): N; Number specifing array size, with 2^N; defaut: N=22\n");
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

   Kokkos::initialize(argc, argv);
   {
#if defined(USE_SERIAL)
   using ExecSpace = Kokkos::Serial;
   using MemSpace  = Kokkos::HostSpace;
#elif defined(USE_OPENMP)
   using ExecSpace = Kokkos::OpenMP;
   using MemSpace  = Kokkos::HostSpace;
#elif defined(USE_OPENMPTARGET)
   using ExecSpace = Kokkos::OpenMPTarget;
   using MemSpace  = Kokkos::OpenMPTargetSpace;
#else  // default: CUDA without UVM
   using ExecSpace = Kokkos::Cuda;
#if defined(USE_CUDAUVM)
   using MemSpace  = Kokkos::CudaUVMSpace;
#else
   using MemSpace  = Kokkos::CudaSpace;
#endif // defined(USE_CUDAUVM)
#endif

#if ! defined(USE_LAYOUTLEFT)
   using Layout = Kokkos::LayoutRight; // for C/C++
#else
   using Layout = Kokkos::LayoutLeft; // for Fortran
#endif

   using range_policy = Kokkos::RangePolicy<ExecSpace>;

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

   // Allocate x, y in device
   using ViewVectorType = Kokkos::View<double *, Layout, MemSpace>;
   ViewVectorType x("x", N);
   ViewVectorType y("y", N);

   // Create host mirros of device 
   ViewVectorType::HostMirror h_x = Kokkos::create_mirror_view( x );
   ViewVectorType::HostMirror h_y = Kokkos::create_mirror_view( y );

   // Initialize vectors on host
   for (int i = 0; i < N; ++i ) {
      h_x( i ) = static_cast<double>(i)/N; 
      h_y( i ) = 0.0; 
   }  

   // Deep copy host view to device view
   Kokkos::deep_copy( x, h_x ); // implicit barrier
   Kokkos::deep_copy( y, h_y ); // implicit barrier

   double elp0 = get_elp_time();

   // Measure kernel
   double a = 1.0e-3 / nitr;

   for ( int itr = 0; itr < nitr; ++itr ) { 

      // daxpy
      //for (int i =0; i<ns; ++i) {
      //  y[i] += a*x[i];
      //}
      Kokkos::parallel_for("daxpy", range_policy(0,N), KOKKOS_LAMBDA( int i ) {
         y( i ) += a*x( i ) ;
      });  

      // a trick for smart compiler
      //a = static_cast<double>(itr % 2); 
      
      Kokkos::fence();  // explicit barrier
   } 

   double time = get_elp_time() - elp0;

   // Deep copy device view to host view
   Kokkos::deep_copy( h_y, y ); // implicit barrier

   double fp_dp, mem_dp;
   double N_dp;

   N_dp = static_cast<double>(N);

   fp_dp  = 2.0*CONV2GIGA*N_dp*nitr;
   mem_dp = 3.0*8.0*CONV2GIGA*N*nitr; 
   fprintf(stdout, "===============================\n");
   fprintf(stdout, "Kokkos execution space %s\n", typeid(ExecSpace).name());
   fprintf(stdout, "===============================\n");
   fprintf(stdout, "Array size                   : %11d\n", N);
   fprintf(stdout, "Num of iterationsn           : %11d\n", nitr);
   fprintf(stdout, "Elapsed time (sec)           : %11.3f\n", time);
   fprintf(stdout, "GFLOP/s (double precision)   : %11.3f\n", fp_dp/time);
   fprintf(stdout, "GB/s    (double precision)   : %11.3f\n", mem_dp/time);
   fprintf(stdout, "===============================\n");

   double ysol;
   fprintf(stdout, "==========CHECK-RESULTS========\n");
   ysol = a * h_x( 0 ) * static_cast<double>(nitr);
   fprintf(stdout, "0  : %.22E\n", h_y( 0 )-ysol); 
   ysol = a * h_x( N/2 ) * static_cast<double>(nitr);
   fprintf(stdout, "N/2: %.22E\n", h_y( N/2 )-ysol); 
   ysol = a * h_x( N-1 ) * static_cast<double>(nitr);
   fprintf(stdout, "N-1: %.22E\n", h_y( N-1 )-ysol); 
   }
   Kokkos::finalize();

   return EXIT_SUCCESS;
}

// Copyright 2025 Research Organization for Information Science and Technology 
// matvec: y <- A*x
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
   int N, M, NEXP, MEXP;
   int nitr;
   const double CONV2GIGA = 1.0e-9;

   if ( argc != 4 ) {
     fprintf(stdout,"[usage] run.x (arg1) (arg2) (arg3)\n");
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

   // Allocate x, y, A in device
   using ViewVectorType = Kokkos::View<double *, Layout, MemSpace>;
   using ViewMatrixType = Kokkos::View<double **, Layout, MemSpace>;
   ViewVectorType x("x", N);
   ViewVectorType y("y", N);
   ViewMatrixType A("A", N, N);

   // Create host mirros of device 
   ViewVectorType::HostMirror h_x = Kokkos::create_mirror_view( x );
   ViewVectorType::HostMirror h_y = Kokkos::create_mirror_view( y );
   ViewMatrixType::HostMirror h_A = Kokkos::create_mirror_view( A );

   // Initialize vectors and matrices on host
   for (int j = 0; j < M; ++j ) {
      h_x( j ) = static_cast<double>(j)/M; 
   } 

   for (int i = 0; i < N; ++i ) {
      h_y( i ) = 0.0; 
   } 

   for (int i = 0; i < N; ++i ) {
   for (int j = 0; j < M; ++j ) {
      h_A( i, j ) = 1.0; 
   }}  

   // Deep copy host view to device view
   Kokkos::deep_copy( x, h_x );
   Kokkos::deep_copy( A, h_A ); // implicit barrier

   double elp0 = get_elp_time();

   // Measure kernel

   for ( int itr = 0; itr < nitr; ++itr ) { 

      // matvec
      //for (int i = 0; i < N; ++i ) {
      //   double tmp = 0.0;
      //   for (int j = 0; j < M; ++j ) {
      //      tmp += A[i][j]*x[j];
      //   }
      //   y[i] = tmp;
      //}
      // naive implementation: this is not suitale for the case large M
      Kokkos::parallel_for("matvec", range_policy(0,N), KOKKOS_LAMBDA( int i ) {
         double tmp = 0.0;
         for ( int j = 0; j < M; ++j ) {
            tmp += A( i, j ) * x(j);
         }
         y( i ) = tmp;
      }); 

      // a trick for smart compiler
      // not yet implemented ...
      
      Kokkos::fence();  // explicit barrier
   } 

   double time = get_elp_time() - elp0;

   // Deep copy device view to host view
   Kokkos::deep_copy( h_y, y ); // implicit barrier

   double fp_dp, mem_dp;
   double N_dp, M_dp;

   N_dp = static_cast<double>(N);
   M_dp = static_cast<double>(M);

   fp_dp  = 2.0*N_dp*M_dp*CONV2GIGA*nitr;
   mem_dp = (8.0*N_dp + 8.0*N_dp*M_dp + 8.0*M_dp)*CONV2GIGA*nitr; 
   fprintf(stdout, "===============================\n");
   fprintf(stdout, "Kokkos execution space %s\n", typeid(ExecSpace).name());
   fprintf(stdout, "===============================\n");
   fprintf(stdout, "Row size                     : %11d\n", N);
   fprintf(stdout, "Column size                  : %11d\n", M);
   fprintf(stdout, "Num of iterationsn           : %11d\n", nitr);
   fprintf(stdout, "Elapsed time (sec)           : %11.3f\n", time);
   fprintf(stdout, "GFLOP/s (double precision)   : %11.3f\n", fp_dp/time);
   fprintf(stdout, "GB/s    (double precision)   : %11.3f\n", mem_dp/time);
   fprintf(stdout, "===============================\n");

   double ysol;
   fprintf(stdout, "==========CHECK-RESULTS========\n");
   ysol = 0.5 * (M - 1);
   fprintf(stdout, "0  : %.22E\n", h_y( 0 )-ysol); 
   ysol = 0.5 * (M - 1);
   fprintf(stdout, "N/2: %.22E\n", h_y( N/2 )-ysol); 
   ysol = 0.5 * (M - 1);
   fprintf(stdout, "N-1: %.22E\n", h_y( N-1 )-ysol); 
   }
   Kokkos::finalize();

   return EXIT_SUCCESS;
}

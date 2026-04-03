// Copyright 2025 Research Organization for Information Science and Technology
// Reference
// https://kokkos.org/kokkos-core-wiki/usecases/OverlappingHostAndDeviceWork.html
// Usage Pattern 1 - overlapping computational kernels
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <Kokkos_Core.hpp>
#include <omp.h>
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


// Setup data on host  
// use parameter xVal to demonstrate variability between iterations
void init_src_views(HostViewVectorFType h_x, HostViewMatrixFType h_A, 
   const FPtype xVal, const int nn, const int mm)
{
  Kokkos::parallel_for( "init_A", Kokkos::RangePolicy<HostExecSpace>(0,nn), 
    KOKKOS_LAMBDA ( const int i ) {
    for ( int j = 0; j < mm; ++j ) {
      h_A( i, j ) = 1.0 / static_cast<FPtype>(mm);
    }
  });

  Kokkos::parallel_for( "init_x", Kokkos::RangePolicy<HostExecSpace>(0,mm), 
    KOKKOS_LAMBDA ( const int i ) {
    h_x( i ) = xVal;
  });
}
 
int main (int argc, char **argv)
{
  int N, M, nrepeat;
  FPtype xv;
  double elp0, elp;

  if ( argc != 4 ) {
     printf("[usage] run.x (arg1) (arg2) (arg3)\n");
     printf("Calculate yAx. y: Nx1, x: Mx1, A: NxM\n");
     printf("        (arg1): Row size (int)\n");
     printf("        (arg2): Column size (int)\n");
     printf("        (arg3): Number of iterations (int)\n");
     return EXIT_SUCCESS;
  }

  N = atoi(argv[1]);
  M = atoi(argv[2]);
  nrepeat = atoi(argv[3]);

  if ( N < 0 ) {
     N = 1 << 10;
  }

  if ( M < 0 ) {
     M = 1 << 12;
  }

  if ( nrepeat < 0 ) {
     nrepeat = 10000;
  }

  printf("Row size                          : %d\n", N);
  printf("Column size                       : %d\n", M);
  printf("Number of iterations              : %d\n", nrepeat);
  {
  int ng;
  CHECK(cudaGetDeviceCount(&ng));
  printf("Number of devices                 : %d\n", ng);
  printf("Kokkos execution space for device : %s\n", typeid(DeviceExecSpace).name());
  int nt;
  nt = omp_get_max_threads();
  printf("Number of threads                 : %d\n", nt);
  printf("Kokkos execution space for host   : %s\n", typeid(HostExecSpace).name());
  }

  Kokkos::initialize(argc, argv);
  {

  ViewVectorFType y( "y", N );
  ViewVectorFType x( "x", M );
  ViewMatrixFType A( "A", N, M );

  HostViewVectorFType h_y = Kokkos::create_mirror_view( y );
  HostViewVectorFType h_x = Kokkos::create_mirror_view( x );
  HostViewMatrixFType h_A = Kokkos::create_mirror_view( A );

  elp0 = get_elp_time();
  for ( int repeat = 0; repeat < nrepeat; repeat++ ) {
     xv = static_cast<FPtype>(repeat+1);
     init_src_views( h_x, h_A, xv, N, M);  // setup data for next device launch
  
     Kokkos::fence(); // barrier used to synch between device and host before copying data
    
     // Deep copy host data needed for this iteration to device.
     Kokkos::deep_copy( h_y, y );
     Kokkos::deep_copy( x, h_x );
     Kokkos::deep_copy( A, h_A );  // implicit barrier

    // Application: y=Ax
    Kokkos::parallel_for( "yAx", Kokkos::RangePolicy<DeviceExecSpace>( 0, N ), KOKKOS_LAMBDA ( const int j ) {
       double temp2 = 0;
       for ( int i = 0; i < M; ++i ) {
         temp2 += A( j, i ) * x( i );
       }
       y( j ) = temp2;
    });
    
    // note that there is no barrier here, so the host thread will loop
    // back around and call ini_src_views while the kernel is still running
#if defined(NO_OVERLAP)
    Kokkos::fence();
#endif    
  }
  Kokkos::fence(); 
  elp = get_elp_time() - elp0;

  Kokkos::deep_copy( h_y, y ); // implicit barrier
 
  FPtype chk0, chk1, chk2;
  chk0 = h_y(0) - static_cast<FPtype>(nrepeat);
  chk1 = h_y(N/2) - static_cast<FPtype>(nrepeat);
  chk2 = h_y(N-1) - static_cast<FPtype>(nrepeat);
  fprintf(stdout, "%-15s %10s %10s %15s %15s %15s\n",
     "type","elp_sec","us_p_itr","chk0","chk1","chk2");
  fprintf(stdout, "%-15s %10.3f %10.3f %15.6E %15.6E %15.6E\n",
    "kokkos",elp,1.0e6*elp/nrepeat,chk0,chk1,chk2);

  } 
  Kokkos::finalize();

  return EXIT_SUCCESS;
}

// Copyright 2025 Research Organization for Information Science and Technology 
//
// STREAM benchmark with Kokkos
//   * COPY:   c <- a
//   * SCALE:  b <- s*c
//   * ADD:    c <- a + b
//   * TRIADD: a <- b + s*c
//
// Reference
// * STREAM (stream.c,v 5.10 2013/01/17 16:01:06 mccalpin Exp mccalpin), 
//   developed by John D. McCalpin; http://www.cs.virginia.edu/stream/
// * STREAM benchmark implementation in CUDA, developed by 
//   Massimiliano Fatica (NVIDIA Corporation) and further modified by
//   Ben Cumming (CSCS) and Andreas Herten (JSC/FZJ);
//   https://github.com/bcumming/cuda-stream
// * Kokkos documentation
//   https://kokkos.org/kokkos-core-wiki/ProgrammingGuide/ParallelDispatch.html
//   https://kokkos.org/kokkos-core-wiki/ProgrammingGuide/ParallelDispatch.html#lambdas
//   https://kokkos.org/kokkos-core-wiki/ProgrammingGuide/HierarchicalParallelism.html
//
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cfloat>
#include <Kokkos_Core.hpp>

#define NTIMES 20

# ifndef MIN
# define MIN(x,y) ((x)<(y)?(x):(y))
# endif
# ifndef MAX
# define MAX(x,y) ((x)>(y)?(x):(y))
# endif

#if defined(USE_SP)
typedef float FPType;
#else
typedef double FPType;
#endif

//--- Define some typedef in Kokkos: Start -----------------------------
typedef Kokkos::Cuda ExecSpace;
typedef Kokkos::CudaSpace MemSpace;
typedef Kokkos::View<FPType *, Kokkos::LayoutRight, MemSpace> ViewVectorType;
typedef ViewVectorType::HostMirror HostViewVectorType;
//--- Define some typedef in Kokkos: End -------------------------------

static double avgtime[4] = {0.0, 0.0, 0.0, 0.0};
static double maxtime[4] = {0.0, 0.0, 0.0, 0.0};
static double mintime[4] = {FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX};

double get_elp_time () {
  struct timespec tp ;
  clock_gettime ( CLOCK_REALTIME, &tp ) ;
  return  tp.tv_sec + (double)tp.tv_nsec*1.0e-9 ;
}

int main (int argc, char **argv)
{
  int nsize;
  int nlg, vl;

  double CONVERT2GIGA = 1.0e-9;
  double bytes[4];
  double times[4][NTIMES];

  FPType scale;
  FPType chk_a, chk_b, chk_c;

  // Parse command-line options
#if 0
  if ( argc != 3 ) {
     fprintf(stdout,"[usage] stream (arg1) (arg2)\n");
     fprintf(stdout,"   (arg1):  Array size (int). if <=0, default (1<<26) is set. \n");
     fprintf(stdout,"   (arg2):  Vector length in Kokkos's hierarchical parallelism (int). if <=0, Kokkos will automatically control everything.\n");
     return EXIT_SUCCESS;
  }
  nsize = atoi(argv[1]);

  if ( nsize <= 0 ) {
     nsize = 1 << 26;
  }

  vl = atoi(argv[2]);
  if ( vl <= 0 ) {
     nlg = 1;
     vl = 0;
  } else {
     nlg = ( nsize % vl == 0 ) ? nsize/vl : nsize/vl + 1;
  }
#else
  if ( argc != 3 ) {
     fprintf(stdout,"[usage] stream (arg1) (arg2)\n");
     fprintf(stdout,"   (arg1):  Array size (int). if <=0, default (1<<26) is set. \n");
     fprintf(stdout,"   (arg2):  dummy option. Always set -1.\n");
     return EXIT_SUCCESS;
  }
  nsize = atoi(argv[1]);

  if ( nsize <= 0 ) {
     nsize = 1 << 26;
  }

  //vl = atoi(argv[2]);
  vl = -1;
  if ( vl <= 0 ) {
     nlg = 1; // never use
     vl = 0;  // never use
  } else {
     nlg = ( nsize % vl == 0 ) ? nsize/vl : nsize/vl + 1; // never use
  }
#endif

  // Start Kokkos scope
  Kokkos::initialize(argc, argv);
  {
  // Settings ...
  bytes[0]= 2.0 * sizeof(FPType) * static_cast<double>(nsize);
  bytes[1]= 2.0 * sizeof(FPType) * static_cast<double>(nsize);
  bytes[2]= 3.0 * sizeof(FPType) * static_cast<double>(nsize);
  bytes[3]= 3.0 * sizeof(FPType) * static_cast<double>(nsize);

  scale = static_cast<FPType>(3.0);

  // For checking the results
  chk_a = static_cast<FPType>(1.0);
  chk_b = static_cast<FPType>(2.0);
  chk_c = static_cast<FPType>(0.0);

  chk_c = chk_a;
  chk_b = scale*chk_c;
  chk_c = chk_a + chk_b;
  chk_a = chk_b + scale*chk_c;

  // View 
  ViewVectorType a("a", nsize);
  ViewVectorType b("b", nsize);
  ViewVectorType c("c", nsize);

  HostViewVectorType h_a = Kokkos::create_mirror_view(a);
  HostViewVectorType h_b = Kokkos::create_mirror_view(b);
  HostViewVectorType h_c = Kokkos::create_mirror_view(c);

  // Start measurement ...
  fprintf(stdout," STREAM Benchmark with Kokkos\n");
  fprintf(stdout," Array size (%s precision) =%7.2f MB\n", 
     sizeof(double)==sizeof(FPType)?"double":"single", double(nsize)*double(sizeof(FPType))/1.e6);

  // Kokkos will automatically control everything.
  fprintf(stdout," Kokkos will automatically control everything.\n");

  for (int k = 0; k < NTIMES; ++k) {
     // Initilize device memory
     Kokkos::parallel_for("init", Kokkos::RangePolicy<ExecSpace>(0, nsize), KOKKOS_LAMBDA (const int i) {
       a( i ) = static_cast<FPType>(1.0);
       b( i ) = static_cast<FPType>(2.0);
       c( i ) = static_cast<FPType>(0.0);
     });
     Kokkos::fence();

     times[0][k] = get_elp_time();
     Kokkos::parallel_for("Copy", Kokkos::RangePolicy<ExecSpace>(0,nsize), KOKKOS_LAMBDA (const int i) {
        c(i) = a(i);
     });
     Kokkos::fence();
     times[0][k] = get_elp_time() - times[0][k];

     times[1][k] = get_elp_time();
     Kokkos::parallel_for("Scale", Kokkos::RangePolicy<ExecSpace>(0,nsize), KOKKOS_LAMBDA (const int i) {
        b(i) = scale*c(i);
     });
     Kokkos::fence();
     times[1][k] = get_elp_time() - times[1][k];

     times[2][k] = get_elp_time();
     Kokkos::parallel_for("Add", Kokkos::RangePolicy<ExecSpace>(0,nsize), KOKKOS_LAMBDA (const int i) {
        c(i) = a(i) + b(i); 
     });
     Kokkos::fence();
     times[2][k] = get_elp_time() - times[2][k];

     times[3][k] = get_elp_time();
     Kokkos::parallel_for("TriAdd", Kokkos::RangePolicy<ExecSpace>(0,nsize), KOKKOS_LAMBDA (const int i) {
        a(i) = b(i) + scale*c(i);
     });
     Kokkos::fence();
     times[3][k] = get_elp_time() - times[3][k];
  } // end of loop-k

#if 1
  Kokkos::parallel_for("debug", Kokkos::RangePolicy<ExecSpace>(0,1), KOKKOS_LAMBDA (const int i) {
     Kokkos::printf("debug-print: a0=%18.6E b0=%18.6E c0=%18.6E\n", a(i), b(i), c(i));
  });
#endif

  Kokkos::deep_copy( h_a, a );
  Kokkos::deep_copy( h_b, b );
  Kokkos::deep_copy( h_c, c ); // implicit barrier

  chk_a = chk_a - h_a( 0 );
  chk_b = chk_b - h_b( 0 );
  chk_c = chk_c - h_c( 0 );

  // Summary 
  for ( int k = 1; k < NTIMES; ++k ) { // skip the first iteration
     for (int j = 0; j < 4; ++j) {
        avgtime[j] = avgtime[j] + times[j][k];
        mintime[j] = MIN(mintime[j], times[j][k]);
        maxtime[j] = MAX(maxtime[j], times[j][k]);
     }
  }

  avgtime[0] = avgtime[0] / static_cast<double>(NTIMES -1);
  avgtime[1] = avgtime[1] / static_cast<double>(NTIMES -1);
  avgtime[2] = avgtime[2] / static_cast<double>(NTIMES -1);
  avgtime[3] = avgtime[3] / static_cast<double>(NTIMES -1);

  fprintf(stdout,"\nFunction      Rate (GB/s)  Avg time(s)  Min time(s)  Max time(s)\n");
  fprintf(stdout,"-----------------------------------------------------------------\n");

  fprintf(stdout,"%8s   %11.4f   %11.8f  %11.8f  %11.8f\n", 
     "Copy", CONVERT2GIGA*bytes[0]/mintime[0], avgtime[0], mintime[0], maxtime[0]);
  fprintf(stdout,"%8s   %11.4f   %11.8f  %11.8f  %11.8f\n", 
     "Scale", CONVERT2GIGA*bytes[1]/mintime[1], avgtime[1], mintime[1], maxtime[1]);
  fprintf(stdout,"%8s   %11.4f   %11.8f  %11.8f  %11.8f\n", 
     "Add", CONVERT2GIGA*bytes[2]/mintime[2], avgtime[2], mintime[2], maxtime[2]);
  fprintf(stdout,"%8s   %11.4f   %11.8f  %11.8f  %11.8f\n", 
     "Triadd", CONVERT2GIGA*bytes[3]/mintime[3], avgtime[3], mintime[3], maxtime[3]);

  fprintf(stdout,"-----------------------------------------------------------------\n");
  fprintf(stdout," # Check the results \n");
  fprintf(stdout,"Diff of a = %18.6E\n", chk_a);
  fprintf(stdout,"Diff of b = %18.6E\n", chk_b);
  fprintf(stdout,"Diff of c = %18.6E\n", chk_c);

  }
  Kokkos::finalize();  // End of Kokkos scope

  return EXIT_SUCCESS;
}

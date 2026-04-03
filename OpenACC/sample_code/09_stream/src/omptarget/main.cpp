// Copyright 2025 Research Organization for Information Science and Technology 
//
// STREAM benchmark with OpenMP Target
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
//
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cfloat>

#define NTIMES 20

# ifndef MIN
# define MIN(x,y) ((x)<(y)?(x):(y))
# endif
# ifndef MAX
# define MAX(x,y) ((x)>(y)?(x):(y))
# endif

#if __CLANG_FUJITSU
#define _restrict_cpp __restrict__
#elif __INTEL_LLVM_COMPILER
#define _restrict_cpp __restrict
#elif __GNUC__
#define _restrict_cpp __restrict__
#endif

#if defined(USE_SP)
typedef float FPType;
#else
typedef double FPType;
#endif

static double avgtime[4] = {0.0, 0.0, 0.0, 0.0};
static double maxtime[4] = {0.0, 0.0, 0.0, 0.0};
static double mintime[4] = {FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX};

double get_elp_time () {
  struct timespec tp ;
  clock_gettime ( CLOCK_REALTIME, &tp ) ;
  return  tp.tv_sec + (double)tp.tv_nsec*1.0e-9 ;
}

//--- Define Kernels: Start --------------------------------------------
void STREAM_Copy (FPType * _restrict_cpp c, FPType * _restrict_cpp a, const int n)
{
   #pragma omp target map(present:c[0:n],a[0:n]) 
   #pragma omp teams distribute parallel for 
   for (int i = 0; i < n ; ++i ) {
       c[i] = a[i]; 
   }
}

void STREAM_Copy2 (FPType * _restrict_cpp c, FPType * _restrict_cpp a, const int n, const int nteams, const int nt)
{
   #pragma omp target map(present:c[0:n],a[0:n]) 
   #pragma omp teams distribute parallel for num_teams(nteams) num_threads(nt)
   for (int i = 0; i < n ; ++i ) {
       c[i] = a[i]; 
   }
}

void STREAM_Scale (FPType * _restrict_cpp b, FPType * _restrict_cpp c, FPType s, const int n)
{
   #pragma omp target map(present:b[0:n],c[0:n]) 
   #pragma omp teams distribute parallel for 
   for (int i = 0; i < n ; ++i ) {
       b[i] = s*c[i]; 
   }
}

void STREAM_Scale2 (FPType * _restrict_cpp b, FPType * _restrict_cpp c, FPType s, const int n, const int nteams, const int nt)
{
   #pragma omp target map(present:b[0:n],c[0:n]) 
   #pragma omp teams distribute parallel for num_teams(nteams) num_threads(nt)
   for (int i = 0; i < n ; ++i ) {
       b[i] = s*c[i]; 
   }
}

void STREAM_Add (FPType * _restrict_cpp c, FPType * _restrict_cpp a, FPType * _restrict_cpp b, const int n)
{
   #pragma omp target map(present:c[0:n],a[0:n],b[0:n]) 
   #pragma omp teams distribute parallel for 
   for (int i = 0; i < n ; ++i ) {
       c[i] = a[i] + b[i]; 
   }
}

void STREAM_Add2 (FPType * _restrict_cpp c, FPType * _restrict_cpp a, FPType * _restrict_cpp b, const int n, const int nteams, const int nt)
{
   #pragma omp target map(present:c[0:n],a[0:n],b[0:n]) 
   #pragma omp teams distribute parallel for num_teams(nteams) num_threads(nt)
   for (int i = 0; i < n ; ++i ) {
       c[i] = a[i] + b[i]; 
   }
}

void STREAM_Triadd (FPType * _restrict_cpp a, FPType * _restrict_cpp b, FPType * _restrict_cpp c, FPType s, const int n)
{
   #pragma omp target map(present:a[0:n],b[0:n],c[0:n]) 
   #pragma omp teams distribute parallel for 
   for (int i = 0; i < n ; ++i ) {
       a[i] = b[i] + s*c[i]; 
   }
}

void STREAM_Triadd2 (FPType * _restrict_cpp a, FPType * _restrict_cpp b, FPType * _restrict_cpp c, FPType s, const int n, const int nteams, const int nt)
{
   #pragma omp target map(present:a[0:n],b[0:n],c[0:n]) 
   #pragma omp teams distribute parallel for num_teams(nteams) num_threads(nt)
   for (int i = 0; i < n ; ++i ) {
       a[i] = b[i] + s*c[i]; 
   }
}

void reset (FPType * _restrict_cpp a, FPType * _restrict_cpp b, FPType * _restrict_cpp c, const int n)
{
   #pragma omp target map(present:c[0:n],a[0:n],b[0:n])
   #pragma omp loop
   for (int i=0; i < n; ++i) {
       a[i] = static_cast<FPType>(1.0);
       b[i] = static_cast<FPType>(2.0);
       c[i] = static_cast<FPType>(0.0);
   }
}
//--- Define Kernels: End ----------------------------------------------


int main (int argc, char **argv)
{
  int nsize;
  int nteams, nt;

  double CONVERT2GIGA = 1.0e-9;
  double bytes[4];
  double times[4][NTIMES];

  FPType scale;
  FPType *a, *b, *c;
  FPType chk_a, chk_b, chk_c;

  // Parse command-line options
  if ( argc != 3 ) {
     fprintf(stdout,"[usage] stream (arg1) (arg2)\n");
     fprintf(stdout,"   (arg1):  Array size (int). if <=0, default (1<<26) is set. \n");
     fprintf(stdout,"   (arg2):  Number of threads per team in device (int). if <=0, Compiler will automatically set it.\n");
     return EXIT_SUCCESS;
  }
  nsize = atoi(argv[1]);

  if ( nsize <= 0 ) {
     nsize = 1 << 26;
  }

  nt = atoi(argv[2]);
  if ( nt <= 0 ) {
     nteams = 1;
     nt = 0;
  } else {
     // nteams = ( nsize % nt == 0 ) ? nsize/nt : nsize/nt + 1; //TODO: To check the result when nsize % nt != 0 
     if ( nsize % nt != 0 ) {
        fprintf(stdout,"Num of threads per team in device is %d.\n", nt);
        fprintf(stdout,"Currently, it must be a divisor of array size, %d.\n", nsize);
	return EXIT_FAILURE;
     }
     nteams = nsize / nt;
  }

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

  a = (FPType *)malloc ( nsize*sizeof(FPType) );
  b = (FPType *)malloc ( nsize*sizeof(FPType) );
  c = (FPType *)malloc ( nsize*sizeof(FPType) );

  // Start measurement ...
  fprintf(stdout," STREAM Benchmark with OpenMP Target\n");
  fprintf(stdout," Array size (%s precision) =%7.2f MB\n", 
     sizeof(double)==sizeof(FPType)?"double":"single", double(nsize)*double(sizeof(FPType))/1.e6);

  //--------------------------------------------------------------------
  if ( nt > 0 ) { // num_teams and num_threads are explicitly set.
  //--------------------------------------------------------------------
  fprintf(stdout," Number of teams: %d, Number of threads per team: %d\n", nteams, nt);
  #pragma omp target data map(alloc:a[0:nsize],b[0:nsize],c[0:nsize])
  {
  for (int k = 0; k < NTIMES; ++k) {
     // Initialize memory of the device
     reset(a,b,c,nsize);

     times[0][k] = get_elp_time();
     STREAM_Copy2(c,a,nsize,nteams,nt);
     times[0][k] = get_elp_time() - times[0][k];

     times[1][k] = get_elp_time();
     STREAM_Scale2(b,c,scale,nsize,nteams,nt);
     times[1][k] = get_elp_time() - times[1][k];

     times[2][k] = get_elp_time();
     STREAM_Add2(c,a,b,nsize,nteams,nt);
     times[2][k] = get_elp_time() - times[2][k];

     times[3][k] = get_elp_time();
     STREAM_Triadd2(a,b,c,scale,nsize,nteams,nt);
     times[3][k] = get_elp_time() - times[3][k];
  } // end of loop-k

#if 1
  #pragma omp target 
  {
  printf("Debug print: a0=%18.6E b0=%18.6E c0=%18.6E\n", a[0], b[0], c[0]);
  }
#endif

  #pragma omp target update from(a[0:1],b[0:1],c[0:1])
  } // end of omp target data
  //--------------------------------------------------------------------
  } else { // num_teams and num_threads are automatically set.
  //--------------------------------------------------------------------
  fprintf(stdout," Number of teams and Number of threads per team are automatically set by compiler.\n");

  #pragma omp target data map(alloc:a[0:nsize],b[0:nsize],c[0:nsize])
  {
  for (int k = 0; k < NTIMES; ++k) {
     // Initialize memory of the device
     reset(a,b,c,nsize);

     times[0][k] = get_elp_time();
     STREAM_Copy(c,a,nsize);
     times[0][k] = get_elp_time() - times[0][k];

     times[1][k] = get_elp_time();
     STREAM_Scale(b,c,scale,nsize);
     times[1][k] = get_elp_time() - times[1][k];

     times[2][k] = get_elp_time();
     STREAM_Add(c,a,b,nsize);
     times[2][k] = get_elp_time() - times[2][k];

     times[3][k] = get_elp_time();
     STREAM_Triadd(a,b,c,scale,nsize);
     times[3][k] = get_elp_time() - times[3][k];
  } // end of loop-k

#if 1
  // for debug
  #pragma omp target
  {
  printf("Debug print: a0=%18.6E b0=%18.6E c0=%18.6E\n", a[0], b[0], c[0]);
  }
#endif

  #pragma omp target update from(a[0:1],b[0:1],c[0:1])
  } // end of omp target data
  //--------------------------------------------------------------------
  } 
  //--------------------------------------------------------------------

  chk_a = chk_a - a[0];
  chk_b = chk_b - b[0];
  chk_c = chk_c - c[0];

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

  free ( a );
  free ( b );
  free ( c );

  return EXIT_SUCCESS;
}

// Copyright 2025 Research Organization for Information Science and Technology 
// matmat: C <- A*B 
//====================================================================
//  cuBLAS
//  https://docs.nvidia.com/cuda/cublas/index.html
//====================================================================
#include <cstdio>
#include <cstdlib>
#include <cuda_runtime.h>
#include <cublas_v2.h>
#include "mykernel.hpp"
#include <openacc.h>
#include "timer.hpp"
#include <typeinfo>

extern void mmp_cuda_wrapper(int, int, int, double *, double *, double *);

int main (int argc, char **argv) 
{
   int N, M, L, NEXP, MEXP, LEXP, nitr;

   const double CONV2GIGA = 1.0e-9;
   const double zero = 0.0;
   const double one  = 1.0;
   double elp0, time;
   double fp_dp, mem_dp;
   double N_dp, M_dp, L_dp;
   double chk0, chk1, chk2;

   cublasStatus_t cublas_stat;
   cublasHandle_t cublas_handle;

   if ( argc != 5 ) {
     fprintf(stdout,"[usage] run.x (arg1) (arg2) (arg3) (arg4)\n");
     fprintf(stdout,"   C = A * B, C: NxM, A=NxL, B=LxM\n");
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

   fprintf(stdout, "#===============================\n");
   fprintf(stdout, "# Setting\n");
   fprintf(stdout, "#===============================\n");
   fprintf(stdout, "Matrix size                  : %11d\n", N);
   fprintf(stdout, "Matrix size                  : %11d\n", M);
   fprintf(stdout, "Matrix size                  : %11d\n", L);
   fprintf(stdout, "Num of iterationsn           : %11d\n", nitr);

   fprintf(stdout, "#===============================\n");
   fprintf(stdout, "# Measurements\n");
   fprintf(stdout, "#===============================\n");

   //
   // For reference
   //
   {
   acc_device_t dev_type = acc_get_device_type();
   int ng = acc_get_num_devices(dev_type);
   fprintf(stdout,"OpenACC on device\n");
   fprintf(stdout,"Number of devices  :   %d\n", ng);
   }

   double *mata, *matb, *matc;
   mata = (double *)malloc( N*L*sizeof(double) );
   matb = (double *)malloc( L*M*sizeof(double) );
   matc = (double *)malloc( N*M*sizeof(double) );

   // Initialize vectors and matrices on host
   for (int i = 0; i < N; ++i ) {
   for (int k = 0; k < L; ++k ) {
      mata[k+L*i] = 1.0; 
   }} 

   for (int k = 0; k < L; ++k ) {
   for (int j = 0; j < M; ++j ) {
      matb[j+M*k] = static_cast<double>(k)/L;
   }} 

   for ( int i = 0; i < N; ++i ) {
   for ( int j = 0; j < M; ++j ) {
       int ji = j + M*i;
       matc[ji] = 0.0;
   }}

   cublas_stat = cublasCreate(&cublas_handle);
   if ( cublas_stat != CUBLAS_STATUS_SUCCESS) {
      fprintf (stderr, "CUBLAS initialization failed\n");
      return EXIT_FAILURE;
   }

   //  [Simple implementation: OpenACC]
   #pragma acc data copyin(mata[0:N*L], matb[0:L*M]) \
   copy(matc[0:N*M])
   {
   elp0 = get_elp_time();

   int icon = 0;
   for ( int it = 0; it < nitr; ++it) {
       mmp_simple_acc(N, M, L, matc, mata, matb);
       icon += dummy(it, N, matc);
   }

   time = get_elp_time() - elp0;
   } // acc data

   chk0 = matc[0] - 0.5 * (L - 1); 
   chk1 = matc[(M/2) + M*(N/2)] - 0.5 * (L - 1); 
   chk2 = matc[M-1+M*(N-1)] - 0.5 * (L - 1); 
   fprintf(stdout, "%-15s %10s %10s %10s %15s %15s %15s\n",
     "type","elp_sec","GFLOPs","GiBs","chk0","chk1","chk2");
   fprintf(stdout, "%-15s %10.3f %10.3f %10.3f %15.6E %15.6E %15.6E\n",
     "simple_acc",time,fp_dp/time,mem_dp/time,chk0,chk1,chk2);

   for ( int i = 0; i < N; ++i ) {
   for ( int j = 0; j < M; ++j ) {
       int ji = j + M*i;
       matc[ji] = 0.0;
   }}

   //  [Simple implementation: OpenACC gang-vector]
   #pragma acc data copyin(mata[0:N*L], matb[0:L*M]) \
   copy(matc[0:N*M])
   {
   elp0 = get_elp_time();

   int icon = 0;
   for ( int it = 0; it < nitr; ++it) {
       mmp_simple_acc_gv(N, M, L, matc, mata, matb);
       icon += dummy(it, N, matc);
   }

   time = get_elp_time() - elp0;
   } // acc data

   chk0 = matc[0] - 0.5 * (L - 1); 
   chk1 = matc[(M/2) + M*(N/2)] - 0.5 * (L - 1); 
   chk2 = matc[M-1+M*(N-1)] - 0.5 * (L - 1); 
   fprintf(stdout, "%-15s %10s %10s %10s %15s %15s %15s\n",
     "type","elp_sec","GFLOPs","GiBs","chk0","chk1","chk2");
   fprintf(stdout, "%-15s %10.3f %10.3f %10.3f %15.6E %15.6E %15.6E\n",
     "simple_acc_gv",time,fp_dp/time,mem_dp/time,chk0,chk1,chk2);

   for ( int i = 0; i < N; ++i ) {
   for ( int j = 0; j < M; ++j ) {
       int ji = j + M*i;
       matc[ji] = 0.0;
   }}

   // [My CUDA implementation]
   #pragma acc data copyin(mata[0:N*L], matb[0:L*M]) \
   copy(matc[0:N*M])
   {
   elp0 = get_elp_time();

   int icon = 0;
   for ( int it = 0; it < nitr; ++it) {
      #pragma acc host_data use_device(mata, matb, matc)
      {
         mmp_cuda_wrapper(N, M, L, matc, mata, matb);
      } // acc host_data 
      icon += dummy(it, N, matc);
   }

   time  = get_elp_time() - elp0;
   } // acc data 

   chk0 = matc[0] - 0.5 * (L - 1); 
   chk1 = matc[(M/2) + M*(N/2)] - 0.5 * (L - 1); 
   chk2 = matc[M-1+M*(N-1)] - 0.5 * (L - 1); 
   fprintf(stdout, "%-15s %10s %10s %10s %15s %15s %15s\n",
     "type","elp_sec","GFLOPs","GiBs","chk0","chk1","chk2");
   fprintf(stdout, "%-15s %10.3f %10.3f %10.3f %15.6E %15.6E %15.6E\n",
     "my_mmp_cuda",time,fp_dp/time,mem_dp/time,chk0,chk1,chk2);

   for ( int i = 0; i < N; ++i ) {
   for ( int j = 0; j < M; ++j ) {
       int ji = j + M*i;
       matc[ji] = 0.0;
   }}

   // [DGEMM in cuBLAS]
   // Note: cuBLAS is implemented by column-major order 
   double *matc_trans = (double *) malloc ( N*M*sizeof(double) );

   for ( int i = 0; i < N; ++i ) {
   for ( int j = 0; j < M; ++j ) {
      int ji = j + M*i;
      int ij = i + N*j;
      matc_trans[ji] = matc[ij];
   }}

   #pragma acc data copyin(mata[0:N*L], matb[0:L*M]) \
   copy(matc_trans[0:M*N])
   {
   elp0 = get_elp_time();

   int icon = 0;
   for ( int it = 0; it < nitr; ++it) {
      #pragma acc host_data use_device(mata, matb, matc_trans)
      {
         cublasDgemm(cublas_handle, CUBLAS_OP_T, CUBLAS_OP_T, N, M, L,
            &one, mata, L, matb, M, &zero, matc_trans, N);
      } // acc host_data 

      const cudaError_t error = cudaDeviceSynchronize();
      if ( error != cudaSuccess) {
         exit(1);
      }
      icon += dummy(it, N, matc_trans);
   }

   time  = get_elp_time() - elp0;
   } // acc data 

   for ( int i = 0; i < N; ++i ) {
   for ( int j = 0; j < M; ++j ) {
       int ji = j + M*i;
       int ij = i + N*j;
       matc[ji] = matc_trans[ij];
   }}

   free( matc_trans );

   chk0 = matc[0] - 0.5 * (L - 1); 
   chk1 = matc[(M/2) + M*(N/2)] - 0.5 * (L - 1); 
   chk2 = matc[M-1+M*(N-1)] - 0.5 * (L - 1); 
   fprintf(stdout, "%-15s %10s %10s %10s %15s %15s %15s\n",
     "type","elp_sec","GFLOPs","GiBs","chk0","chk1","chk2");
   fprintf(stdout, "%-15s %10.3f %10.3f %10.3f %15.6E %15.6E %15.6E\n",
     "cublas",time,fp_dp/time,mem_dp/time,chk0,chk1,chk2);

   free( mata );
   free( matb );
   free( matc );

   return EXIT_SUCCESS;
}

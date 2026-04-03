/* Copyright 2024 Research Organization for Information Science and Technology */
/* ==================================================================== 
 * Reference: 
 * * Intel MPI Benchmarks (v2021.3); http://github.com/intel/mpi-benchmarks
 * * J. Cheng, M. Grossman, T. McKercher, "Professional CUDA C 
 *   Programming" (2014) Chap.9.
 * * OpenMPI FAQ: Running CUDA-aware Open MPI;
 *   https://www.open-mpi.org/faq/?category=runcuda#mpi-cuda-dev
 * * KOKKOS package in LAMMPS
 *   https://docs.lammps.org/Speed_kokkos.html
 * /home/7/us03787/project/LAMMPS/lammps-2Aug2023/src/KOKKOS
 *
 * Note: The communication steps are GPU0 -> D2H -> rank 0 -> MPI_Isend
 * -> rank 1 -> H2D -> GPU1.
 * ==================================================================== */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cuda_runtime_api.h>

#if (OPEN_MPI)

#if (OMPI_MAJOR_VERSION >= 2)
#include <mpi-ext.h> /* Needed for CUDA-aware check */

#if defined(MPIX_CUDA_AWARE_SUPPORT) && MPIX_CUDA_AWARE_SUPPORT
int have_cuda_aware = 1;
#elif defined(MPIX_CUDA_AWARE_SUPPORT) && !MPIX_CUDA_AWARE_SUPPORT
int have_cuda_aware = 0;
#else
int have_cuda_aware = -1; /* unknown */
#endif /* defined(MPIX_CUDA_AWARE_SUPPORT) */

#else /*(OMPI_MAJOR_VERSION < 2) */

int have_cuda_aware = 0;

#endif /*(OMPI_MAJOR_VERSION >= 2)*/
#endif /* OPEN_MPI */

#define MAX_MSG_SIZE (1<<22)
#define BUFFER_SIZE MAX_MSG_SIZE
#define MAX_NITR 10000

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

#define CHECK_MPI(call)                                                             \
{                                                                                   \
   cudaError_t error = call;                                                        \
   if ( error != cudaSuccess )                                                      \
   {                                                                                \
      fprintf(stderr,"Error: %s:%d ", __FILE__, __LINE__ );                         \
      fprintf(stderr,"returned %d -> %s\n", (int)error, cudaGetErrorString(error)); \
      MPI_Finalize();                                                               \
      exit(EXIT_FAILURE);                                                           \
   }                                                                                \
}

int main (int argc, char **argv)
{
   int NITR;
   int me, np;
   int source, dest;
   int igpu, ng;

   double elp0, elp;

   MPI_Status stat;

   MPI_Init(NULL, NULL);
   MPI_Comm_rank(MPI_COMM_WORLD, &me);
   MPI_Comm_size(MPI_COMM_WORLD, &np);

   ng = 0;
   CHECK_MPI(cudaGetDeviceCount(&ng));

   if ( np != 2 ) {
     if ( me == 0 ) {
        fprintf(stdout,"Error: Number of MPI tasks is strictly set as 2 in this code\n");
     }
     MPI_Finalize();
     return EXIT_FAILURE;
   }

   if ( ng < 2 ) {
      if ( me == 0 ) {
         fprintf(stdout,"Error: Number of available GPUS must be 2 at least in this code\n");
      }
      MPI_Finalize();
      return EXIT_FAILURE;
   }

   if ( have_cuda_aware == 1 ) {
      if ( me == 0 ) {
         fprintf(stdout,"MPI library has CUDA-aware support.\n");
      }
   } else if ( have_cuda_aware == 0 ) {
      if ( me == 0 ) {
         fprintf(stdout,"MPI library does not have CUDA-aware support.\n");
      }
   } else {
      if ( me < 0 ) {
         fprintf(stdout,"MPI library cannot determine if there is CUDA-aware support.\n");
      }
   }

   dest = ( me > 0 ) ? 0 : 1;
   source = ( me > 0 ) ? 0 : 1;

   igpu = ( (me == 1) ? 0 : 1);
   CHECK_MPI(cudaSetDevice(igpu));

   fprintf(stdout,"Rank %d uses GPU %d of %d GPUs.\n", me, igpu, ng);

   char *src_buf, *rcv_buf;
   src_buf = (char *)malloc( sizeof(char)*(BUFFER_SIZE+1) );
   rcv_buf = (char *)malloc( sizeof(char)*(BUFFER_SIZE+1) );

   char *src_buf_d, *rcv_buf_d;
   CHECK_MPI(cudaMalloc((void **)&src_buf_d, BUFFER_SIZE+1));
   CHECK_MPI(cudaMalloc((void **)&rcv_buf_d, BUFFER_SIZE+1));

   memset(src_buf, '0', BUFFER_SIZE );
   memset(rcv_buf, '1', BUFFER_SIZE );
   CHECK_MPI(cudaMemcpy(src_buf_d, src_buf, BUFFER_SIZE, cudaMemcpyDefault));
   CHECK_MPI(cudaMemcpy(rcv_buf_d, rcv_buf, BUFFER_SIZE, cudaMemcpyDefault));

   if ( me == 0 ) {
      fprintf(stdout,"msg_bytes ave_latency_sec ave_bandwidth_mbps\n");
   }

   if ( have_cuda_aware == 1 ) {
      for ( int size = 1 ; size < MAX_MSG_SIZE; size *= 2)
      {
         if ( size > 4096 ) {
            NITR = MAX_NITR;
         } else if ( size > 65535 ) {
            NITR = MAX_NITR / 10;
         } else if ( size > 1048576 ) {
            NITR = MAX_NITR / 100;
         } else {
            NITR = 20;
         }

         MPI_Barrier(MPI_COMM_WORLD);

         if ( me == 0 ) {
            elp0 = MPI_Wtime();
            for (int i = 0; i < NITR; ++i ) {
               MPI_Send (src_buf_d, size, MPI_CHAR, dest, 100, MPI_COMM_WORLD);
               MPI_Recv (rcv_buf_d, size, MPI_CHAR, source, 101, MPI_COMM_WORLD, &stat);
            }
            elp = MPI_Wtime() - elp0;
            elp = elp / NITR;
         } else if ( me == 1 ) {
            elp0 = MPI_Wtime();
            for (int i = 0; i < NITR; ++i ) {
               MPI_Recv (rcv_buf_d, size, MPI_CHAR, source, 100, MPI_COMM_WORLD, &stat);
               MPI_Send (src_buf_d, size, MPI_CHAR, dest, 101, MPI_COMM_WORLD);
            }
            elp = MPI_Wtime() - elp0;
            elp = elp / NITR;
         } 

         MPI_Barrier(MPI_COMM_WORLD);
         double elpavg;
         MPI_Reduce(&elp, &elpavg, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

         if ( me == 0 ) {
            elpavg *= 0.5;
            double bwavg = ((double)size/ 1048576.0)/elpavg;
            fprintf(stdout,"%10d %14.3e %14.5f\n", 
                    size,elpavg,bwavg);
         }

      } /* size */
   } else { /* have_cuda_aware <= 0 */
      for ( int size = 1 ; size < MAX_MSG_SIZE; size *= 2)
      {
         if ( size > 4096 ) {
            NITR = MAX_NITR;
         } else if ( size > 65535 ) {
            NITR = MAX_NITR / 10;
         } else if ( size > 1048576 ) {
            NITR = MAX_NITR / 100;
         } else {
            NITR = 20;
         }

         MPI_Barrier(MPI_COMM_WORLD);

         if ( me == 0 ) {
            elp0 = MPI_Wtime();
            for (int i = 0; i < NITR; ++i ) {
               /* D2H */
               CHECK_MPI(cudaMemcpy(src_buf, src_buf_d, size, cudaMemcpyDeviceToHost));

               MPI_Send (src_buf, size, MPI_CHAR, dest, 100, MPI_COMM_WORLD);
               MPI_Recv (rcv_buf, size, MPI_CHAR, source, 101, MPI_COMM_WORLD, &stat);
               /* H2D */
               CHECK_MPI(cudaMemcpy(rcv_buf_d, rcv_buf, size, cudaMemcpyHostToDevice));
            }
            elp = MPI_Wtime() - elp0;
            elp = elp / NITR;
         } else if ( me == 1 ) {
            elp0 = MPI_Wtime();
            for (int i = 0; i < NITR; ++i ) {
               MPI_Recv (rcv_buf, size, MPI_CHAR, source, 100, MPI_COMM_WORLD, &stat);
               /* H2D */
               CHECK_MPI(cudaMemcpy(rcv_buf_d, rcv_buf, size, cudaMemcpyHostToDevice));
	       /* D2H */
               CHECK_MPI(cudaMemcpy(src_buf, src_buf_d, size, cudaMemcpyDeviceToHost));
               MPI_Send (src_buf, size, MPI_CHAR, dest, 101, MPI_COMM_WORLD);
            }
            elp = MPI_Wtime() - elp0;
            elp = elp / NITR;
         } 

         MPI_Barrier(MPI_COMM_WORLD);
         double elpavg;
         MPI_Reduce(&elp, &elpavg, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

         if ( me == 0 ) {
            elpavg *= 0.5;
            double bwavg = ((double)size/ 1048576.0)/elpavg;
            fprintf(stdout,"%10d %14.3e %14.5f\n", 
                    size,elpavg,bwavg);
         }

      } /* size */
   } /* end: have_cuda_aware == 1 */

   free( src_buf );
   free( rcv_buf );
   CHECK_MPI(cudaFree(src_buf_d));
   CHECK_MPI(cudaFree(rcv_buf_d));

   MPI_Finalize();
   return EXIT_SUCCESS;
}

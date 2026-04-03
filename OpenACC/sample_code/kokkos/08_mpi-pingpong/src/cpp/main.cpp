// Copyright 2025 Research Organization for Information Science and Technology 
// ==================================================================== 
// Reference: 
//  * Intel MPI Benchmarks (v2021.3); http://github.com/intel/mpi-benchmarks
//  * J. Cheng, M. Grossman, T. McKercher, "Professional CUDA C 
//    Programming" (2014) Chap.9.
//  * MPI Halo Exchange; https://kokkos.org/kokkos-core-wiki/usecases/MPI-Halo-Exchange.html
//
//  Note: The communication steps are GPU0 -> D2H -> rank 0 -> MPI_Isend
//  -> rank 1 -> H2D -> GPU1.
// ==================================================================== 
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <Kokkos_Core.hpp>
#include <mpi.h>
#include <typeinfo>
#include "types.hpp"

//#include <cuda_runtime_api.h>

#if (OPEN_MPI)

#if (OMPI_MAJOR_VERSION >= 2)
#include <mpi-ext.h> // Needed for CUDA-aware check 

#if defined(MPIX_CUDA_AWARE_SUPPORT) && MPIX_CUDA_AWARE_SUPPORT
int have_cuda_aware = 1;
#elif defined(MPIX_CUDA_AWARE_SUPPORT) && !MPIX_CUDA_AWARE_SUPPORT
int have_cuda_aware = 0;
#else
int have_cuda_aware = -1; // unknown
#endif // defined(MPIX_CUDA_AWARE_SUPPORT)

#else // (OMPI_MAJOR_VERSION < 2) 
int have_cuda_aware = 0;
#endif // (OMPI_MAJOR_VERSION >= 2)

#endif // OPEN_MPI

#define MAX_MSG_SIZE (1<<22)
#define BUFFER_SIZE MAX_MSG_SIZE
#define MAX_NITR 10000

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
         fprintf(stdout,"Error: Number of available GPUS must be greater than or equal to 2.\n");
      }
      MPI_Finalize();
      return EXIT_FAILURE;
   }

   // Initializing Kokkos setting
   Kokkos::InitializationSettings settings;
   
   settings.set_num_threads(1);

   igpu = ( (me == 1) ? 0 : 1);
   settings.set_device_id(igpu);

   Kokkos::initialize(settings);
   {

   dest = ( me > 0 ) ? 0 : 1;
   source = ( me > 0 ) ? 0 : 1;

   fprintf(stdout,"Rank %d uses GPU %d of %d GPUs.\n", me, igpu, ng);
   fprintf(stdout,"Cuda-aware MPI (1: Yes, 0: No, -1: Unknown): %d\n", have_cuda_aware);
   fprintf(stdout,"Kokkos execution space: %s\n", typeid(ExecSpace).name());

   ViewVectorCType src_buf_d("src_buf_d", BUFFER_SIZE+1 );
   ViewVectorCType rcv_buf_d("src_buf_d", BUFFER_SIZE+1 );

   HostViewVectorCType src_buf, rcv_buf;

   src_buf = Kokkos::create_mirror_view( src_buf_d );
   rcv_buf = Kokkos::create_mirror_view( rcv_buf_d );

   for ( int i = 0 ; i < BUFFER_SIZE; ++i ) {
     src_buf(i) = '0';
     rcv_buf(i) = '1';
   }

   Kokkos::deep_copy( src_buf_d, src_buf);
   Kokkos::deep_copy( rcv_buf_d, rcv_buf);

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
               MPI_Send (src_buf_d.data(), size, MPI_CHAR, dest, 100, MPI_COMM_WORLD);
               MPI_Recv (rcv_buf_d.data(), size, MPI_CHAR, source, 101, MPI_COMM_WORLD, &stat);
            }
            elp = MPI_Wtime() - elp0;
            elp = elp / NITR;
         } else if ( me == 1 ) {
            elp0 = MPI_Wtime();
            for (int i = 0; i < NITR; ++i ) {
               MPI_Recv (rcv_buf_d.data(), size, MPI_CHAR, source, 100, MPI_COMM_WORLD, &stat);
               MPI_Send (src_buf_d.data(), size, MPI_CHAR, dest, 101, MPI_COMM_WORLD);
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

      } // size
   } else { // have_cuda_aware <= 0 
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
               // D2H 
               Kokkos::deep_copy(src_buf, src_buf_d);

               MPI_Send (src_buf.data(), size, MPI_CHAR, dest, 100, MPI_COMM_WORLD);
               MPI_Recv (rcv_buf.data(), size, MPI_CHAR, source, 101, MPI_COMM_WORLD, &stat);

               // H2D 
               Kokkos::deep_copy(rcv_buf_d, rcv_buf);
            }
            elp = MPI_Wtime() - elp0;
            elp = elp / NITR;
         } else if ( me == 1 ) {
            elp0 = MPI_Wtime();
            for (int i = 0; i < NITR; ++i ) {
               MPI_Recv (rcv_buf.data(), size, MPI_CHAR, source, 100, MPI_COMM_WORLD, &stat);
               // H2D 
               Kokkos::deep_copy(rcv_buf_d, rcv_buf);
	       // D2H
               Kokkos::deep_copy(src_buf, src_buf_d);
               MPI_Send (src_buf.data(), size, MPI_CHAR, dest, 101, MPI_COMM_WORLD);
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

      } // size 
   } // end: have_cuda_aware == 1 
   }  
   Kokkos::finalize();

   MPI_Finalize();
   return EXIT_SUCCESS;
}

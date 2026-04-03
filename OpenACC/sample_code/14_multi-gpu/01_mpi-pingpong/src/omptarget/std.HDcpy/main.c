/* Copyright 2025 Research Organization for Information Science and Technology */
/* ==================================================================== 
 * Reference: 
 * * Intel MPI Benchmarks (v2021.3); http://github.com/intel/mpi-benchmarks
 * * J. Cheng, M. Grossman, T. McKercher, "Professional CUDA C 
 *   Programming" (2014) Chap.9.
 *
 * Note: The communication steps are GPU0 -> D2H -> rank 0 -> MPI_Isend
 * -> rank 1 -> H2D -> GPU1.
 * ==================================================================== */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <omp.h>

#define MAX_MSG_SIZE (1<<22)
#define BUFFER_SIZE MAX_MSG_SIZE
#define MAX_NITR 10000

int main (int argc, char **argv)
{
   int NITR;
   int me, np;
   int src_tag, rcv_tag;
   int source, dest;
   int igpu, ng, ihost;

   double elp0, elp;

   MPI_Status stat;

   MPI_Init(NULL, NULL);
   MPI_Comm_rank(MPI_COMM_WORLD, &me);
        MPI_Comm_size(MPI_COMM_WORLD, &np);

   ihost = omp_get_initial_device();
   ng = omp_get_num_devices();

   igpu = ( (me == 1) ? 1 : 0);

   if ( np != 2 ) {
      if ( me == 0 ) {
         fprintf(stdout,"Error: Number of MPI tasks is strictly set as 2 in this code\n");
      }
      MPI_Finalize();
      return EXIT_FAILURE;
   }

   if ( ng == 1 && np == 2 ) { /* Special case */
      ng = 2;
      if ( me == 0 ) {
         fprintf(stdout,"Warning: PPN must be 1 in your setting. The program will be continuned ...\n");
      }
      igpu = 0; /* since only 1 GPU exists */
   }

   if ( ng < 2 ) {
      if ( me == 0 ) {
         fprintf(stdout,"Error: Number of available GPUs must be 2 at least in this code\n");
      }
      MPI_Finalize();
      return EXIT_FAILURE;
   }

   dest = ( me > 0 ) ? 0 : 1;
   source = ( me > 0 ) ? 0 : 1;

   omp_set_default_device(igpu);

   fprintf(stdout,"Rank %d uses GPU %d of %d GPUs.\n", me, igpu, ng);
   fprintf(stdout,"Rank %d has initial host ID %d .\n", me, ihost);

   char *src_buf, *rcv_buf;
   src_buf = (char *)malloc( sizeof(char)*(BUFFER_SIZE+1) );
   rcv_buf = (char *)malloc( sizeof(char)*(BUFFER_SIZE+1) );

   char *src_buf_d, *rcv_buf_d;
   src_buf_d = (char *)omp_target_alloc( sizeof(char)*(BUFFER_SIZE+1), igpu );
   rcv_buf_d = (char *)omp_target_alloc( sizeof(char)*(BUFFER_SIZE+1), igpu );

   memset(src_buf, '0', BUFFER_SIZE );
   memset(rcv_buf, '1', BUFFER_SIZE );
   omp_target_memcpy(src_buf_d, src_buf, BUFFER_SIZE, 0, 0, igpu, ihost);
   omp_target_memcpy(rcv_buf_d, rcv_buf, BUFFER_SIZE, 0, 0, igpu, ihost);

   if ( me == 0 ) {
      fprintf(stdout,"msg_bytes ave_latency_sec ave_bandwidth_mbps\n");
   }

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
            omp_target_memcpy(src_buf, src_buf_d, size, 0, 0, ihost, igpu);

            MPI_Send (src_buf, size, MPI_CHAR, dest, 100, MPI_COMM_WORLD);
            MPI_Recv (rcv_buf, size, MPI_CHAR, source, 101, MPI_COMM_WORLD, &stat);

            /* H2D */
            omp_target_memcpy(rcv_buf_d, rcv_buf, size, 0, 0, igpu, ihost);
         }

         elp = MPI_Wtime() - elp0;
	 elp = elp / NITR;

      } else if ( me == 1 ) {

         elp0 = MPI_Wtime();

         for (int i = 0; i < NITR; ++i ) {
            MPI_Recv (rcv_buf, size, MPI_CHAR, source, 100, MPI_COMM_WORLD, &stat);

            /* H2D */
            omp_target_memcpy(rcv_buf_d, rcv_buf, size, 0, 0, igpu, ihost);

	    /* D2H */
            omp_target_memcpy(src_buf, src_buf_d, size, 0, 0, ihost, igpu);

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

   free( src_buf );
   free( rcv_buf );
   omp_target_free( src_buf_d, igpu );
   omp_target_free( rcv_buf_d, igpu );

   MPI_Finalize();

   return EXIT_SUCCESS;
}


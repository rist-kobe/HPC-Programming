/* Copyright 2025 Research Organization for Information Science and Technology */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MSG_SIZE (1<<22)
#define BUFFER_SIZE MAX_MSG_SIZE
#define MAX_NITR 10000

int main (int argc, char **argv)
{
   int NITR;
   int me, np;
   int source, dest;

   double elp0, elp;

   MPI_Status stat;

   MPI_Init(NULL, NULL);
   MPI_Comm_rank(MPI_COMM_WORLD, &me);
   MPI_Comm_size(MPI_COMM_WORLD, &np);

   if ( np != 2 ) {
     if ( me == 0 ) {
        fprintf(stdout,"Error: Number of MPI tasks is strictly set as 2 in this code\n");
     }
     MPI_Finalize();
     return EXIT_FAILURE;
   }

   char *src_buf, *rcv_buf;
   src_buf = (char *)malloc( sizeof(char)*(BUFFER_SIZE+1) );
   rcv_buf = (char *)malloc( sizeof(char)*(BUFFER_SIZE+1) );

   memset(src_buf, '0', BUFFER_SIZE );

   dest = ( me > 0 ) ? 0 : 1;
   source = ( me > 0 ) ? 0 : 1;

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

     /* Blocking synchronous send */
     if ( me == 0 ) {
        elp0 = MPI_Wtime();
        for (int i = 0; i < NITR; ++i ) {
           MPI_Ssend (src_buf, size, MPI_CHAR, dest, 100, MPI_COMM_WORLD);
           MPI_Recv (rcv_buf, size, MPI_CHAR, source, 101, MPI_COMM_WORLD, &stat);
        }
        elp = MPI_Wtime() - elp0;
        elp = elp / NITR;
     } else if ( me == 1 ) {
        elp0 = MPI_Wtime();
        for (int i = 0; i < NITR; ++i ) {
           MPI_Recv (rcv_buf, size, MPI_CHAR, source, 100, MPI_COMM_WORLD, &stat);
           MPI_Ssend (src_buf, size, MPI_CHAR, dest, 101, MPI_COMM_WORLD);
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

   MPI_Finalize();
   return EXIT_SUCCESS;
}

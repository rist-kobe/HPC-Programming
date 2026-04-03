/* Copyright 2025 Research Organization for Information Science and Technology */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openacc.h>

#define MAX_MSG_SIZE 100
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

   acc_device_t dev_type = acc_get_device_type();
   ng = acc_get_num_devices(dev_type);

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

   acc_set_device_num(igpu, dev_type);

   fprintf(stdout,"Rank %d uses GPU %d of %d GPUs.\n", me, igpu, ng);

   int *src_buf, *rcv_buf;
   src_buf = (int *)malloc( sizeof(int)*(BUFFER_SIZE+1) );
   rcv_buf = (int *)malloc( sizeof(int)*(BUFFER_SIZE+1) );

   int *src_buf_d, *rcv_buf_d;
   src_buf_d = (int *)acc_malloc( sizeof(int)*(BUFFER_SIZE+1) );
   rcv_buf_d = (int *)acc_malloc( sizeof(int)*(BUFFER_SIZE+1) );

   // set send buf
   const int nelm = 4;
   src_buf[0] = me;      // my MPI rank
   src_buf[1] = igpu;    // GPU id bindint to my MPI rank
   src_buf[2] = -1;      // initial host id
   src_buf[3] = nelm;    // number of elements

   // set rcv buf
   for (int i = 0; i < nelm; ++i ){
      rcv_buf[i] = 100 - me;
   }

   acc_memcpy_to_device(src_buf_d, src_buf, BUFFER_SIZE);
   acc_memcpy_to_device(rcv_buf_d, rcv_buf, BUFFER_SIZE);

   FILE *fp;
   char fn[32];
   sprintf(fn, "chk.%03d", me);
   fp = fopen(fn, "w");

   fprintf(fp,"## Before ping-pong\n");
   fprintf(fp,"* send buffer \n");
   for ( int i =0; i< nelm; ++i ) {
      fprintf(fp,"%d %d\n", i, src_buf[i]);
   }
   fprintf(fp,"* receive buffer \n");
   for ( int i =0; i< nelm; ++i ) {
      fprintf(fp,"%d %d\n", i, rcv_buf[i]);
   }

   // a single ping-pong
   {
      const int size = nelm;

      MPI_Barrier(MPI_COMM_WORLD);

      if ( me == 0 ) {

            /* D2H */
            acc_memcpy_from_device(src_buf, src_buf_d, size);

            MPI_Send (src_buf, size, MPI_INT, dest, 100, MPI_COMM_WORLD);
            MPI_Recv (rcv_buf, size, MPI_INT, source, 101, MPI_COMM_WORLD, &stat);

            /* H2D */
            acc_memcpy_to_device(rcv_buf_d, rcv_buf, size);


      } else if ( me == 1 ) {


            MPI_Recv (rcv_buf, size, MPI_INT, source, 100, MPI_COMM_WORLD, &stat);

            /* H2D */
            acc_memcpy_to_device(rcv_buf_d, rcv_buf, size);

            /* D2H */
            acc_memcpy_from_device(src_buf, src_buf_d, size);

            MPI_Send (src_buf, size, MPI_INT, dest, 101, MPI_COMM_WORLD);

      }

      MPI_Barrier(MPI_COMM_WORLD);
   } 

   fprintf(fp,"## After ping-pong\n");
   fprintf(fp,"* receive buffer \n");
   for ( int i =0; i< nelm; ++i ) {
      fprintf(fp,"%d %d\n", i, rcv_buf[i]);
   }

   free( src_buf );
   free( rcv_buf );
   acc_free( src_buf_d );
   acc_free( rcv_buf_d );

   MPI_Finalize();

   return EXIT_SUCCESS;
}


/* Copyright 2025 Research Organization for Information Science and Technology */
/*----------------------------------------------------------------------
  Title:       Jacobi method (2-dim. model, dynamical memory allocation, OpenMP Target)
  Author:      Yukihiro Ota (yota@rist.or.jp)
  Last update: May 13rd, 2025
  Reference:   
    [1] M. Sugihara and K. Murota, "Theoretical Numerical Linear 
    Algebra" (Iwanami,2009) [in Japanese].
    [2] Lis; https://www.ssisc.org/lis/index.ex.html
  --------------------------------------------------------------------*/
#include <math.h>
#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "timer.h"

#if defined(USE_NVTX)
#include <nvtx3/nvToolsExt.h>
#endif

#define MAXITR 100

#define IDX(I,J,LDJ) ( (J) + (I)*(LDJ) )
#define MAX(a, b) ( (a) < (b) ? (b) : (a) )

int main (int argc, char **argv)
{
  int nx, ny;
  int np, npx, npy, me, provided;

  int itr;
  int iconv;

  double tol = 1.0e-6;
  double chg = 1.0;
  double nrmsq, nrmsq_loc;
  double nrmbsq;
  double elp0;
  double elp[3];
  /*-------------------------------------------------------------------
   * Parse command-line arguments
   *-----------------------------------------------------------------*/
  nx = 8192;
  ny = 8192;
  npx = 2;
  npy = 2;
  if ( argc == 5 ) {
     nx = atoi(argv[1]);
     ny = atoi(argv[2]);
     npx = atoi(argv[3]);
     npy = atoi(argv[4]);
  }
  /*-------------------------------------------------------------------
   * Initialize
   *-----------------------------------------------------------------*/
  MPI_Init_thread(NULL,NULL,MPI_THREAD_SINGLE,&provided);
  MPI_Comm_size(MPI_COMM_WORLD, &np);
  MPI_Comm_rank(MPI_COMM_WORLD, &me);
  if ( MPI_THREAD_SINGLE < provided ) {
     if ( me == 0 ) {
        fprintf(stdout,"Error: Please check thread level in your MPI\n");
        fprintf(stdout,"Required: %d\n", MPI_THREAD_SINGLE);
        fprintf(stdout,"Provided: %d\n", provided);
     }
     MPI_Finalize();
     return EXIT_FAILURE;
  }
  if ( npx == 1 ) npy = np;
  if ( np != npx*npy ) {
     if ( me == 0 ) {
        fprintf(stdout,"Error: npx * npy must be equal to the number of MPI processes.\n");
        fprintf(stdout,"Your npx (=arg1): %d\n", npx);
        fprintf(stdout,"Your npy (=arg2): %d\n", npy);
     }
     MPI_Finalize();
     return EXIT_FAILURE;
  }
  /*-------------------------------------------------------------------
   * GPU Affinity
   *-----------------------------------------------------------------*/
  int ng = 0;
  int igpu = 0;
  ng = omp_get_num_devices();
  igpu = me%ng;
  omp_set_default_device(igpu);
  /*-------------------------------------------------------------------
   * Initial host ID
   *-----------------------------------------------------------------*/
  int ihost;
  ihost = omp_get_initial_device();
  /*-------------------------------------------------------------------
   * 2D Cartesian topology 
   *-----------------------------------------------------------------*/
  int ndims, reorder;
  int dims[2], periods[2], coords[2];
  MPI_Comm cartComm;

  ndims = 2;
  periods[0] = 0;
  periods[1] = 0;
  reorder = 0;
  dims[0] = npx;
  dims[1] = npy;
  /* Use of Dims_create would be preferable 
   * MPI_Dims_create(np,ndims, dims); */

  MPI_Cart_create(MPI_COMM_WORLD, ndims, &dims[0], &periods[0], reorder, &cartComm);
  MPI_Cart_coords(cartComm, me, 2, coords);

  int disp_x, disp_y;
  int rank_src_xdown, rank_dest_xdown, rank_src_ydown, rank_dest_ydown;
  int rank_src_xup, rank_dest_xup, rank_src_yup, rank_dest_yup;

  disp_x = -1; /* downwards */
  disp_y = -1; /* downwards */
  MPI_Cart_shift(cartComm, 0, disp_x, &rank_src_xdown, &rank_dest_xdown);
  MPI_Cart_shift(cartComm, 1, disp_y, &rank_src_ydown, &rank_dest_ydown);

  disp_x = 1; /* upwards */
  disp_y = 1; /* upwards */
  MPI_Cart_shift(cartComm, 0, disp_x, &rank_src_xup, &rank_dest_xup);
  MPI_Cart_shift(cartComm, 1, disp_y, &rank_src_yup, &rank_dest_yup);
  /*-------------------------------------------------------------------
   * Decomposition
   * Interior domain of coordinates
   *   ix in [1, nx_intr=nx_loc-2]
   *   iy in [1, ny_intr=nx_loc-2]
   * Halo domain
   *   ix = 0, nx_loc-1, iy in [1, ny_intr]
   *   iy = 0, ny_loc-1, ix in [1, nx_intr]
   *-----------------------------------------------------------------*/
  int q, r;
  int nx_intr, nx_halo, nx_loc, offset_intr_x, offset_loc_x;
  int ny_intr, ny_halo, ny_loc, offset_intr_y, offset_loc_y;
 
  q = nx / dims[0];
  r = nx % dims[0];
  nx_halo = 2; /* -1, +1 */
  if ( coords[0] < r ) {
     nx_intr = q + 1;
     nx_loc = nx_intr + nx_halo;
     offset_intr_x = 0 + coords[0]*nx_intr;
     offset_loc_x = 1 + coords[0]*nx_loc;
  } else {
     nx_intr = q;
     nx_loc = nx_intr + nx_halo;
     offset_intr_x = 0 + coords[0]*nx_intr + r;
     offset_loc_x = 1 + coords[0]*nx_loc + r;
  }

  q = ny / dims[1];
  r = ny % dims[1];
  ny_halo = 2; /* -1, +1 */
  if ( coords[1] < r ) {
     ny_intr = q + 1;
     ny_loc = ny_intr + ny_halo;
     offset_intr_y = 0 + coords[1]*ny_intr;
     offset_loc_y = 1 + coords[1]*ny_loc;
  } else {
     ny_intr = q;
     ny_loc = ny_intr + ny_halo;
     offset_intr_y = 0 + coords[1]*ny_intr + r;
     offset_loc_y = 1 + coords[1]*ny_loc + r;
  }

  int max_buffer_size;
  q = (nx/npx) + 1;
  r = (ny/npy) + 1;
  max_buffer_size = MAX(q, r);
  double *phie = create_dp_1d_array(nx_loc * ny_loc);
  double *phio = create_dp_1d_array(nx_loc * ny_loc);
  double *rho = create_dp_1d_array(nx_loc * ny_loc);

  double *send_buff = create_dp_1d_array(max_buffer_size);
  double *recv_buff = create_dp_1d_array(max_buffer_size);

  int tag = 99;
  MPI_Status status;
#if 0
  /* for debug */
  FILE *fp;
  char fname[128];
  sprintf(fname,"chk.%04d", me);
  fp = fopen(fname, "w");
  fprintf(fp, "rank            =%d\n", me);
  fprintf(fp, "cart coord      =%d, %d\n", coords[0], coords[1]);
  fprintf(fp, "rank_src_xdown  =%d\n", rank_src_xdown);
  fprintf(fp, "rank_dest_xdown =%d\n", rank_dest_xdown);
  fprintf(fp, "rank_src_ydown  =%d\n", rank_src_ydown);
  fprintf(fp, "rank_dest_ydown =%d\n", rank_dest_ydown);
  fprintf(fp, "rank_src_xup    =%d\n", rank_src_xup);
  fprintf(fp, "rank_dest_xup   =%d\n", rank_dest_xup);
  fprintf(fp, "rank_src_yup    =%d\n", rank_src_yup);
  fprintf(fp, "rank_dest_yup   =%d\n", rank_dest_yup);
  fprintf(fp, "nx_intr         =%d\n", nx_intr);
  fprintf(fp, "nx_loc          =%d\n", nx_loc);
  fprintf(fp, "offset_intr_x   =%d\n", offset_intr_x);
  fprintf(fp, "offset_loc_x    =%d\n", offset_loc_x);
  fprintf(fp, "ny_intr         =%d\n", ny_intr);
  fprintf(fp, "ny_loc          =%d\n", ny_loc);
  fprintf(fp, "offset_intr_y   =%d\n", offset_intr_y);
  fprintf(fp, "offset_loc_y    =%d\n", offset_loc_y);
  fprintf(fp, "max_buffer_size =%d\n", max_buffer_size);
  fclose(fp);
#endif
#if 0
  /* for debug */
  FILE *fp;
  char fname[128];
  sprintf(fname,"chk.%04d", me);
  fp = fopen(fname, "w");
  fprintf(fp, "rank            =%d\n", me);
  fprintf(fp, "cart coord      =%d, %d\n", coords[0], coords[1]);
  fprintf(fp, "nx/2            =%d\n", nx/2);
  fprintf(fp, "ny/2            =%d\n", ny/2);
  fprintf(fp, "offset_intr_x           =%d\n", offset_intr_x);
  fprintf(fp, "offset_intr_x+nx_intr   =%d\n", offset_intr_x+nx_intr);
  fprintf(fp, "offset_intr_y           =%d\n", offset_intr_y);
  fprintf(fp, "offset_intr_y+ny_intr   =%d\n", offset_intr_y+ny_intr);
  if (  (offset_intr_x <= nx/2) && (offset_intr_y <= ny/2) 
      && (offset_intr_x+nx_intr > nx/2) && (offset_intr_y+ny_intr > ny/2) ) {
     fprintf(fp, "interior index of nx/2  =%d\n", nx/2 - offset_intr_x);
     fprintf(fp, "local index of nx/2     =%d\n", nx/2 - offset_intr_x + offset_loc_x - offset_intr_x);
     fprintf(fp, "interior index of ny/2  =%d\n", ny/2 - offset_intr_y);
     fprintf(fp, "local index of ny/2     =%d\n", ny/2 - offset_intr_y + offset_loc_y - offset_intr_y);
  }
#endif
  /*-------------------------------------------------------------------
   * Pre-processing
   *-----------------------------------------------------------------*/
  elp0 = get_elp_time();

  /* initialize */
  for ( int ix = 1; ix < nx_loc-1; ++ix ) {
  for ( int iy = 1; iy < ny_loc-1; ++iy ) {
     rho[IDX(ix,iy,ny_loc)] = 0.0;
     phie[IDX(ix,iy,ny_loc)] = 0.0;
     phio[IDX(ix,iy,ny_loc)] = 0.0;
  }}

  if (  (offset_intr_x <= nx/2) && (offset_intr_y <= ny/2) 
      && (offset_intr_x+nx_intr > nx/2) && (offset_intr_y+ny_intr > ny/2) ) {
    int dofx = offset_loc_x - offset_intr_x;
    int dofy = offset_loc_y - offset_intr_y;
    rho[IDX(nx/2-offset_intr_x+dofx,ny/2-offset_loc_y+dofy,ny_loc)] = chg;
  }

  for ( int ix = 1; ix < nx_loc-1; ++ix ) {
  for ( int iy = 1; iy < ny_loc-1; ++iy ) {
      phie[IDX(ix,iy,ny_loc)] = chg * 1.0e-2;
      phio[IDX(ix,iy,ny_loc)] = 0.0;
  }}
  
  if( coords[1] == 0 || coords[1] == npy-1) {
    for ( int ix = 1; ix < nx_loc-1; ++ix ) {
       phie[IDX(ix,1,ny_loc)] = 0.0; // boundary condition
       phie[IDX(ix,ny_loc-2,ny_loc)] = 0.0; // boundary condition
    }
  }

  if( coords[0] == 0 || coords[0] == npx-1) {
    for ( int iy = 1; iy < ny_loc-1; ++iy ) {
       phie[IDX(1,iy,ny_loc)] = 0.0; // boundary condition
       phie[IDX(nx_loc-2,iy,ny_loc)] = 0.0; // boundary condition
    }
  }

  nrmbsq = chg;

  elp[0] = get_elp_time() - elp0;

  MPI_Barrier(MPI_COMM_WORLD);
  /*-------------------------------------------------------------------
   * Main solver
   *-----------------------------------------------------------------*/
  elp0 = get_elp_time();

  /* main loop */
#if defined(USE_NVTX)
  nvtxRangeId_t id0 = nvtxRangeStartA("data");
#endif
  #pragma omp target data map(to:rho[0:nx_loc*ny_loc]) map(tofrom:phie[0:nx_loc*ny_loc]) \
  map(alloc:send_buff[0:max_buffer_size],recv_buff[0:max_buffer_size],phio[0:nx_loc*ny_loc])
  {

  #pragma omp target map(present:phio[0:nx_loc*ny_loc])
  #pragma omp loop collapse(2)
  for (int ix = 0; ix < nx_loc; ++ix ) {
    for (int iy = 0; iy < ny_loc; ++iy ) {
    phio[IDX(ix,iy,ny_loc)] = 0.0;
  }}

#if defined(USE_NVTX)
  nvtxRangeId_t id1 = nvtxRangeStartA("rept");
#endif
  iconv = 0;
  for ( itr = 1; itr <= MAXITR; ++itr) {

#if defined(USE_NVTX)
  nvtxRangeId_t id2 = nvtxRangeStartA("update");
#endif
    nrmsq_loc = 0.0;
    #pragma omp target map(tofrom:nrmsq_loc) \
    map(present:phio[0:nx_loc*ny_loc],phie[0:nx_loc*ny_loc],rho[0:nx_loc*ny_loc]) 
    #pragma omp loop collapse(2) reduction(max:nrmsq_loc)
    for (int ix = 1; ix < nx_loc-1; ++ix ) {
      for (int iy = 1; iy < ny_loc-1; ++iy ) {
        phio[IDX(ix,iy,ny_loc)] = 0.25 * ( 
            phie[IDX(ix+1,iy,ny_loc)] + phie[IDX(ix-1,iy,ny_loc)] 
          + phie[IDX(ix,iy+1,ny_loc)] + phie[IDX(ix,iy-1,ny_loc)] 
          + rho[IDX(ix,iy,ny_loc)]);
        nrmsq_loc = fmax(nrmsq_loc, 
          fabs(phio[IDX(ix,iy,ny_loc)] - phie[IDX(ix,iy,ny_loc)]));
      }
    }
#if defined(USE_NVTX)
  nvtxRangeEnd(id2); 
#endif

  if( coords[1] == 0 || coords[1] == npy-1) {
    #pragma omp target data map(tofrom:phio[0:nx_loc*ny_loc])
    #pragma omp target
    #pragma omp loop 
    for ( int ix = 1; ix < nx_loc-1; ++ix ) {
       phio[IDX(ix,1,ny_loc)] = 0.0; // boundary condition
       phio[IDX(ix,ny_loc-2,ny_loc)] = 0.0; // boundary condition
    }
  }

  if( coords[0] == 0 || coords[0] == npx-1) {
    #pragma omp target data map(tofrom:phio[0:nx_loc*ny_loc])
    #pragma omp target
    #pragma omp loop 
    for ( int iy = 1; iy < ny_loc-1; ++iy ) {
       phio[IDX(1,iy,ny_loc)] = 0.0; // boundary condition
       phio[IDX(nx_loc-2,iy,ny_loc)] = 0.0; // boundary condition
    }
  }

#if defined(USE_NVTX)
  nvtxRangeId_t id3 = nvtxRangeStartA("swap");
#endif
    #pragma omp target map(present:phie[0:nx_loc*ny_loc],rho[0:nx_loc*ny_loc])
    #pragma omp loop collapse(2)
    for (int ix = 1; ix < nx_loc-1; ++ix ) {
      for (int iy = 1; iy < ny_loc-1; ++iy ) {
        phie[IDX(ix,iy,ny_loc)] = phio[IDX(ix,iy,ny_loc)];
      }
    }
#if defined(USE_NVTX)
  nvtxRangeEnd(id3);
#endif

    MPI_Allreduce(&nrmsq_loc, &nrmsq, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    if ( tol*nrmbsq >= nrmsq ) {
      iconv = 1;
      break;
    }

    /* p2p communication along -x                                      */
    /* packing */
    #pragma omp target map(present:phie[0:nx_loc*ny_loc],send_buff[0:max_buffer_size])
    #pragma omp loop
    for (int iy = 1; iy < ny_loc-1; ++iy) {
       send_buff[iy] = phie[IDX(1,iy,ny_loc)];
    }

    #pragma omp target update from(send_buff[0:ny_loc-2])
    
    MPI_Sendrecv(&send_buff[0], ny_loc-2, MPI_DOUBLE, rank_dest_xdown, tag, 
                 &recv_buff[0], ny_loc-2, MPI_DOUBLE, rank_src_xdown, tag,
                 cartComm, &status);
     
    #pragma omp target update to(recv_buff[0:ny_loc-2])

    /* unpacking */
    #pragma omp target map(present:phio[0:nx_loc*ny_loc],recv_buff[0:max_buffer_size])
    #pragma omp loop 
    for (int iy = 1; iy < ny_loc-1; ++iy) {
       phio[IDX(nx_loc-1,iy,ny_loc)] = recv_buff[iy];
    }

    /* p2p communication along +x                                      */
    /* packing */
    #pragma omp target map(present:phie[0:nx_loc*ny_loc],send_buff[0:max_buffer_size])
    #pragma omp loop
    for (int iy = 1; iy < ny_loc-1; ++iy) {
       send_buff[iy] = phie[IDX(nx_loc-2,iy,ny_loc)];
    }

    #pragma omp target update from(send_buff[0:ny_loc-2])
    
    MPI_Sendrecv(&send_buff[0], ny_loc-2, MPI_DOUBLE, rank_dest_xup, tag, 
                 &recv_buff[0], ny_loc-2, MPI_DOUBLE, rank_src_xup, tag,
                 cartComm, &status);
     
    #pragma omp target update to(recv_buff[0:ny_loc-2])

    /* unpacking */
    #pragma omp target map(present:phio[0:nx_loc*ny_loc],recv_buff[0:max_buffer_size])
    #pragma omp loop 
    for (int iy = 1; iy < ny_loc-1; ++iy) {
       phio[IDX(0,iy,ny_loc)] = recv_buff[iy];
    }

    /* p2p communication along -y                                      */
    /* packing */
    #pragma omp target map(present:phie[0:nx_loc*ny_loc],send_buff[0:max_buffer_size])
    #pragma omp loop
    for (int ix = 1; ix < nx_loc-1; ++ix) {
       send_buff[ix] = phie[IDX(ix,1,ny_loc)];
    }

    #pragma omp target update from(send_buff[0:nx_loc-2])
    
    MPI_Sendrecv(&send_buff[0], nx_loc-2, MPI_DOUBLE, rank_dest_ydown, tag, 
                 &recv_buff[0], nx_loc-2, MPI_DOUBLE, rank_src_ydown, tag,
                 cartComm, &status);
     
    #pragma omp target update to(recv_buff[0:nx_loc-2])

    /* unpacking */
    #pragma omp target map(present:phio[0:nx_loc*ny_loc],recv_buff[0:max_buffer_size])
    #pragma omp loop 
    for (int ix = 1; ix < nx_loc-1; ++ix) {
       phio[IDX(ix,ny_loc-1,ny_loc)] = recv_buff[ix];
    }

    /* p2p communication along +y                                      */
    /* packing */
    #pragma omp target map(present:phie[0:nx_loc*ny_loc],send_buff[0:max_buffer_size])
    #pragma omp loop
    for (int ix = 1; ix < nx_loc-1; ++ix) {
       send_buff[ix] = phie[IDX(ix,ny_loc-2,ny_loc)];
    }

    #pragma omp target update from(send_buff[0:nx_loc-2])
    
    MPI_Sendrecv(&send_buff[0], nx_loc-2, MPI_DOUBLE, rank_dest_yup, tag, 
                 &recv_buff[0], nx_loc-2, MPI_DOUBLE, rank_src_yup, tag,
                 cartComm, &status);
     
    #pragma omp target update to(recv_buff[0:nx_loc-2])

    /* unpacking */
    #pragma omp target map(present:phio[0:nx_loc*ny_loc],recv_buff[0:max_buffer_size])
    #pragma omp loop 
    for (int ix = 1; ix < nx_loc-1; ++ix) {
       phio[IDX(ix,0,ny_loc)] = recv_buff[ix];
    }

  } /* itr */
#if defined(USE_NVTX)
  nvtxRangeEnd(id1);
#endif
  } /* omp target data */
#if defined(USE_NVTX)
  nvtxRangeEnd(id0);
#endif

  elp[1] = get_elp_time() - elp0;

  if ( iconv ) {
    if ( me == 0 ) {
       printf ("Convergence\n");
       printf ("Itr. count=%d\n", --itr);
    }
  } else {
    if ( me == 0 ) {
       printf ("Not Convergence\n");
       printf ("Itr. count=%d\n", --itr);
    }
  }

  elp0 = get_elp_time();
  /*-------------------------------------------------------------------
   * Post processing
   *-----------------------------------------------------------------*/
  /* finalize */
#if 0
  FILE *fp;
  fp = fopen ("phi.dat","w");
  for ( int ix = 0; ix < NX; ++ix ) {
    for ( int iy = 0; iy < NY; ++iy ) {
      fprintf (fp,"%d %d %13.4f\n", ix, iy, phie[IDX(ix,iy)]);
    }
    fprintf (fp, "\n"); 
  }
  fprintf (fp,"%d %d %13.4f\n", NY/2, NY/2, phie[IDX(NX/2,NY/2)]);

  fclose (fp);
#endif

  elp[2] = get_elp_time() - elp0;

  if ( me == 0 ) {
     fprintf (stdout, "init (host)                  : %13.4f sec.\n",elp[0]);
     fprintf (stdout, "main loop (device, OMPTARGET): %13.4f sec.\n",elp[1]);
     fprintf (stdout, "i/o  (host)                  : %13.4f sec.\n",elp[2]);
  }

  release_dp_1d_array( phie ); 
  release_dp_1d_array( phio ); 
  release_dp_1d_array( rho  ); 

  release_dp_1d_array( send_buff );
  release_dp_1d_array( recv_buff );
  /*-------------------------------------------------------------------
   * Finalize
   *-----------------------------------------------------------------*/
  MPI_Finalize();

  return EXIT_SUCCESS;
}

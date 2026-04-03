/* Copyright 2024 Research Organization for Information Science and Technology */
/*----------------------------------------------------------------------
  Title:       Jacobi method (2-dim. model, dynamical memory allocation, OpenACC)
  Author:      Yukihiro Ota (yota@rist.or.jp)
  Last update: 2nd Feb. 2024
  Reference:   
    [1] M. Sugihara and K. Murota, "Theoretical Numerical Linear 
    Algebra" (Iwanami,2009) [in Japanese].
    [2] Lis; https://www.ssisc.org/lis/index.ex.html
  --------------------------------------------------------------------*/
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "timer.h"

#define NX 8192
#define NY 8192
#define MAXITR 100

#define IDX(I,J) ( (J) + (I)*(NY) )

int main (int argc, char **argv)
{
  int itr;
  int iconv;

  double tol = 1.0e-6;
  double chg = 1.0;
  double nrmsq;
  double nrmbsq;
  double elp0;
  double elp[3];

  double *phie = create_dp_1d_array(NX*NY);
  double *phio = create_dp_1d_array(NX*NY);
  double *rho  = create_dp_1d_array(NX*NY);

  elp0 = get_elp_time();

  /* initialize */
  #pragma omp parallel for schedule(static) shared(rho) 
  for ( int ix = 0; ix < NX; ++ix ) {
    for ( int iy = 0; iy < NY; ++iy ) {
      rho[IDX(ix,iy)] = 0.0;
    }
  }
  rho[IDX(NX/2,NY/2)] = chg;

  #pragma omp parallel for schedule(static) shared(phie,phio) 
  for ( int ix = 0; ix < NX; ++ix ) {
    for ( int iy = 0; iy < NY; ++iy ) {
      phie[IDX(ix,iy)] = chg * 1.0e-2;
      phio[IDX(ix,iy)] = 0.0;
    }
  }

  #pragma omp parallel for schedule(static) shared(phie)  
  for ( int ix = 0; ix < NX; ++ix ) {
    phie[IDX(ix,0)] = 0.0;
    phie[IDX(ix,NY-1)] = 0.0;
  }

  #pragma omp parallel for schedule(static) shared(phie)  
  for ( int iy = 0; iy < NY; ++iy ) {
    phie[IDX(0,iy)] = 0.0;
    phie[IDX(NX-1,iy)] = 0.0;
  }

  nrmbsq = chg;

  elp[0] = get_elp_time() - elp0;

  elp0 = get_elp_time();

  /* main loop */
  iconv = 0;
  for ( itr = 1; itr <= MAXITR; ++itr) {

    nrmsq = 0.0;
    #pragma acc data copyin(phie[0:NX*NY],rho[0:NX*NY]) copyout(phio[0:NX*NY])
    #pragma acc kernels
    #pragma acc loop independent reduction(max:nrmsq)
    for (int ix = 1; ix < NX-1; ++ix ) {
      for (int iy = 1; iy < NY-1; ++iy ) {
        phio[IDX(ix,iy)] = 0.25 * ( phie[IDX(ix+1,iy)] + phie[IDX(ix-1,iy)] 
                              + phie[IDX(ix,iy+1)] + phie[IDX(ix,iy-1)] 
                              + rho[IDX(ix,iy)]);
        nrmsq = fmax(nrmsq, fabs(phio[IDX(ix,iy)] - phie[IDX(ix,iy)]));
      }
    }
#if 0
    for (int ix = 0; ix < NX; ++ix ) {
      phio[IDX(ix,0)] = 0.0;
      phio[IDX(ix,NY-1)] = 0.0;
    }

    for (int iy = 0; iy < NY; ++iy ) {
      phio[IDX(0,iy)] = 0.0;
      phio[IDX(NX-1,iy)] = 0.0;
    }
#endif
    #pragma acc data copyin(phio[0:NX*NY]) copyout(phie[0:NX*NY])
    #pragma acc kernels
    #pragma acc loop independent 
    for ( int ix = 0; ix < NX; ++ix ) {
      for ( int iy = 0; iy < NY; ++iy ) {
        phie[IDX(ix,iy)] = phio[IDX(ix,iy)];
      }
    }

    if ( tol*nrmbsq >= nrmsq ) {
      iconv = 1;
      break;
    }
  } /* itr */

  elp[1] = get_elp_time() - elp0;

  if ( iconv ) {
    printf ("Convergence\n");
    printf ("Itr. count=%d\n", --itr);
  } else {
    printf ("Not Convergence\n");
    printf ("Itr. count=%d\n", --itr);
  }

  elp0 = get_elp_time();

  /* finalize */
  FILE *fp;
  fp = fopen ("phi.dat","w");
#if 0
  for ( int ix = 0; ix < NX; ++ix ) {
    for ( int iy = 0; iy < NY; ++iy ) {
      fprintf (fp,"%d %d %13.4f\n", ix, iy, phie[IDX(ix,iy)]);
    }
    fprintf (fp, "\n"); 
  }
#endif
  fprintf (fp,"%d %d %13.4f\n", NX/2, NY/2, phie[IDX(NX/2,NY/2)]);

  fclose (fp);

  elp[2] = get_elp_time() - elp0;

  fprintf (stdout, "init (host,OMP)        : %13.4f sec.\n",elp[0]);
  fprintf (stdout, "main loop (device, ACC): %13.4f sec.\n",elp[1]);
  fprintf (stdout, "i/o  (host)            : %13.4f sec.\n",elp[2]);

  release_dp_1d_array( phie ); 
  release_dp_1d_array( phio ); 
  release_dp_1d_array( rho  ); 

  return EXIT_SUCCESS;
}

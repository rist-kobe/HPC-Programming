/* Copyright 2026 Research Organization for Information Science and Technology */
/*----------------------------------------------------------------------
  Title:       Jacobi method (2-dim. model)
  Author:      Yukihiro Ota (yota@rist.or.jp)
  Last update: March 16th, 2026
  Reference:   
    [1] M. Sugihara and K. Murota, "Theoretical Numerical Linear 
    Algebra" (Iwanami,2009) [in Japanese].
    [2] Lis; https://www.ssisc.org/lis/index.ex.html
  --------------------------------------------------------------------*/
#include "memory.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define NX 1024
#define NY 1024
#define MAXITR 1000

int main (int argc, char **argv)
{
  int itr;
  int iconv;

  double tol = 1.0e-3;
  double chg = 1.0;
  double nrmsq;
  double nrmbsq;
  double elp0;
  double elp[3];
#if defined(USE_STATICMEM)
  /* For static memory */
  double phie[NX][NY];
  double phio[NX][NY];
  double rho[NX][NY];
#else
  double **phie = create_dp_2d_array(NX, NY);
  double **phio = create_dp_2d_array(NX, NY);
  double **rho  = create_dp_2d_array(NX, NY);
#endif

  elp0 = omp_get_wtime ();

  /* initialize */
#pragma omp parallel for schedule(static) shared(rho) 
  for ( int ix = 0; ix < NX; ++ix ) {
    for ( int iy = 0; iy < NY; ++iy ) {
      rho[ix][iy] = 0.0;
    }
  }
  rho[NX/2][NY/2] = chg;

  nrmbsq = 0.0;
#pragma omp parallel for schedule(static) shared(rho) \
reduction(+:nrmbsq)
  for ( int ix = 0; ix < NX; ++ix ) {
    for ( int iy = 0; iy < NY; ++iy ) {
      nrmbsq += rho[ix][iy] * rho[ix][iy];
    }
  }

#pragma omp parallel for schedule(static) shared(phie,phio) 
  for ( int ix = 0; ix < NX; ++ix ) {
    for ( int iy = 0; iy < NY; ++iy ) {
      phie[ix][iy] = chg * 1.0e-2;
      phio[ix][iy] = 0.0;
    }
  }

#pragma omp parallel for schedule(static) shared(phie)  
  for ( int ix = 0; ix < NX; ++ix ) {
    phie[ix][0] = 0.0;
    phie[ix][NY-1] = 0.0;
  }

#pragma omp parallel for schedule(static) shared(phie)  
  for ( int iy = 0; iy < NY; ++iy ) {
    phie[0][iy] = 0.0;
    phie[NX-1][iy] = 0.0;
  }

  elp[0] = omp_get_wtime () - elp0;

  elp0 = omp_get_wtime ();

  /* main loop */
  iconv = 0;
  for ( itr = 1; itr <= MAXITR; ++itr) {

#pragma omp parallel for schedule(static) shared(phio,phie,rho) 
    for (int ix = 1; ix < NX-1; ++ix ) {
      for (int iy = 1; iy < NY-1; ++iy ) {
        phio[ix][iy] = 0.25 * ( phie[ix+1][iy] + phie[ix-1][iy] 
                              + phie[ix][iy+1] + phie[ix][iy-1] 
                              + rho[ix][iy]);
      }
    }

#pragma omp parallel for schedule(static) shared(phio) 
    for (int ix = 0; ix < NX; ++ix ) {
      phio[ix][0] = 0.0;
      phio[ix][NY-1] = 0.0;
    }

#pragma omp parallel for schedule(static) shared(phio)  
    for (int iy = 0; iy < NY; ++iy ) {
      phio[0][iy] = 0.0;
      phio[NX-1][iy] = 0.0;
    }

    nrmsq = 0.0;
#pragma omp parallel for schedule(static) shared(phio,phie) \
reduction(+:nrmsq)
    for ( int ix = 0; ix < NX; ++ix ) {
      for ( int iy = 0; iy < NY; ++iy ) {
        nrmsq += 16.0 * (phio[ix][iy] - phie[ix][iy])
                      * (phio[ix][iy] - phie[ix][iy]);
      }
    }

#pragma omp parallel for schedule(static) shared(phie,phio) 
    for ( int ix = 0; ix < NX; ++ix ) {
      for ( int iy = 0; iy < NY; ++iy ) {
        phie[ix][iy] = phio[ix][iy];
      }
    }

    if ( tol*nrmbsq >= nrmsq ) {
      iconv = 1;
      break;
    }
  } /* itr */

  elp[1] = omp_get_wtime () - elp0;

  if ( iconv ) {
    printf ("Convergence\n");
    printf ("Itr. count=%d\n", --itr);
  } else {
    printf ("Not Convergence\n");
    printf ("Itr. count=%d\n", --itr);
  }

  elp0 = omp_get_wtime ();

  /* finalize */
  FILE *fp;
  fp = fopen ("phi.dat","w");

  for ( int ix = 0; ix < NX; ++ix ) {
    for ( int iy = 0; iy < NY; ++iy ) {
      fprintf (fp,"%d %d %13.4f\n", ix, iy, phie[ix][iy]);
    }
    fprintf (fp, "\n"); 
  }

  fclose (fp);

  elp[2] = omp_get_wtime () - elp0;

  fprintf (stdout, "init     : %13.4f sec.\n",elp[0]);
  fprintf (stdout, "main loop: %13.4f sec.\n",elp[1]);
  fprintf (stdout, "i/o      : %13.4f sec.\n",elp[2]);

#if ! defined(USE_STATICMEM)
  release_dp_2d_array( phie ); 
  release_dp_2d_array( phio ); 
  release_dp_2d_array( rho  ); 
#endif

  return EXIT_SUCCESS;
}

/* Copyright 2024 Research Organization for Information Science and Technology */
/*----------------------------------------------------------------------
  Title:       Jacobi method (2-dim. model)
  Author:      Yukihiro Ota (yota@rist.or.jp)
  Last update: July 22nd, 2026
  Reference:   
    [1] M. Sugihara and K. Murota, "Theoretical Numerical Linear 
    Algebra" (Iwanami,2009) [in Japanese].
    [2] Lis; https://www.ssisc.org/lis/index.ex.html
  --------------------------------------------------------------------*/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define NX 1024
#define NY 1024
#define MAXITR 1000

int main (void)
{
  int itr;
  int iconv;

  double tol = 1.0e-3;
  double chg = 1.0;
  double nrmsq;
  double nrmbsq;
  double elp0;
  double elp[3];
  double phia[NX][NY];
  double phib[NX][NY];
  double rho[NX][NY];
  /* Double buffering without the O(NX*NY) copy of the plain serial
   * version: phi points to the current iterate and phinew to the
   * next one; the two pointers are swapped after every sweep.      */
  double (*phi)[NY] = phia;
  double (*phinew)[NY] = phib;
  double (*ptmp)[NY];

  elp0 = omp_get_wtime ();

  /* initialize */
  for ( int ix = 0; ix < NX; ++ix ) {
  for ( int iy = 0; iy < NY; ++iy ) {
    rho[ix][iy] = 0.0;
  }}
  /* A point charge at the grid-center cell (NX/2, NY/2 in 0-based
   * indexing; the same cell as NX/2+1, NY/2+1 in the 1-based Fortran
   * version).                                                       */
  rho[NX/2][NY/2] = chg;

  nrmbsq = 0.0;
  for ( int ix = 0; ix < NX; ++ix ) {
  for ( int iy = 0; iy < NY; ++iy ) {
    nrmbsq += rho[ix][iy] * rho[ix][iy];
  }}

  for ( int ix = 0; ix < NX; ++ix ) {
  for ( int iy = 0; iy < NY; ++iy ) {
    phi[ix][iy] = chg * 1.0e-2;
    phinew[ix][iy] = 0.0;
  }}

  /* Dirichlet boundary condition: phi = 0 on the whole boundary.
   * Both buffers get zero boundaries here, so the boundary never
   * needs to be touched again inside the iteration loop.           */
  for ( int ix = 0; ix < NX; ++ix ) {
    phi[ix][0] = 0.0;
    phi[ix][NY-1] = 0.0;
    phinew[ix][0] = 0.0;
    phinew[ix][NY-1] = 0.0;
  }

  for ( int iy = 0; iy < NY; ++iy ) {
    phi[0][iy] = 0.0;
    phi[NX-1][iy] = 0.0;
    phinew[0][iy] = 0.0;
    phinew[NX-1][iy] = 0.0;
  }

  elp[0] = omp_get_wtime () - elp0;

  elp0 = omp_get_wtime ();

  /* main loop */
  iconv = 0;
  for ( itr = 1; itr <= MAXITR; ++itr) {

    for (int ix = 1; ix < NX-1; ++ix ) {
    for (int iy = 1; iy < NY-1; ++iy ) {
      phinew[ix][iy] = 0.25 * ( phi[ix+1][iy] + phi[ix-1][iy] 
                              + phi[ix][iy+1] + phi[ix][iy-1] 
                              + rho[ix][iy]);
    }}

    /* Residual test: || A x - b ||^2 <= tol * || b ||^2. The factor
     * 16 = 4^2 rescales the difference of the two iterates by the
     * diagonal entry of the discrete Laplacian.                     */
    nrmsq = 0.0;
    for ( int ix = 0; ix < NX; ++ix ) {
    for ( int iy = 0; iy < NY; ++iy ) {
      nrmsq += 16.0 * (phinew[ix][iy] - phi[ix][iy])
                    * (phinew[ix][iy] - phi[ix][iy]);
    }}

    /* Swap the buffers instead of copying phinew back to phi. */
    ptmp = phi;
    phi = phinew;
    phinew = ptmp;

    if ( tol*nrmbsq >= nrmsq ) {
      iconv = 1;
      break;
    }
  } /* itr */

  elp[1] = omp_get_wtime () - elp0;

  --itr;
  if ( iconv ) {
    printf ("Convergence\n");
    printf ("Itr. count=%d\n", itr);
  } else {
    printf ("Not Convergence\n");
    printf ("Itr. count=%d\n", itr);
  }

  elp0 = omp_get_wtime ();

  /* finalize: phi always points to the latest iterate. */
  FILE *fp;
  fp = fopen ("phi.dat","w");

  for ( int ix = 0; ix < NX; ++ix ) {
    for ( int iy = 0; iy < NY; ++iy ) {
      fprintf (fp,"%d %d %13.4f\n", ix, iy, phi[ix][iy]);
    }
    fprintf (fp, "\n"); 
  }

  fclose (fp);

  elp[2] = omp_get_wtime () - elp0;

  fprintf (stdout, "init     : %13.4f sec.\n",elp[0]);
  fprintf (stdout, "main loop: %13.4f sec.\n",elp[1]);
  fprintf (stdout, "i/o      : %13.4f sec.\n",elp[2]);

  return EXIT_SUCCESS;
}

/* Copyright 2024 Research Organization for Information Science and Technology */
/*------------------------------------------------------------------------
  Address mapping of 2D array in C
  Author:      Yukihiro Ota (yota@rist.or.jp)
  Last update: 27 Oct 2017
------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main ( int argc, char **argv ) 
{
  const int nrow = 3 ;  
  const int ncol = 8 ;  
  double   x_s[nrow][ncol] ; /* static        */
  double** x_d ;             /* dynamical (1) */
  double** x_dc_ptr ;        /* dynamical (2) */
  double*  x_dc ;

  /* Memory allocation                                                   */
  x_d = (double **)malloc ( sizeof(double *)*nrow ) ;
  for ( int i=0; i<nrow; ++i ) {
    x_d[i] = (double *)malloc ( sizeof(double)*ncol ) ;
  }

  x_dc_ptr = (double **)malloc ( sizeof(double *)*nrow ) ;
  x_dc = (double *)malloc ( sizeof(double)*nrow*ncol ) ; /* 1D array */
  for ( int i=0; i<nrow; ++i ) {
    x_dc_ptr[i] = x_dc + i*ncol ; /* pointer to 1D array per row */
  }
  /*---------------------------------------------------------------------*/
  /* Initialization of data with random number in [0,1)                   */
  /*---------------------------------------------------------------------*/
  {
    srand( (unsigned int)time(NULL) ) ;
    for ( int i=0; i<nrow; ++i ) {
      for ( int j=0; j<ncol; ++j ) {
        double xtmp = 2.0*(double )rand()/RAND_MAX - 1.0 ;
        x_s[i][j]  = xtmp ;
        x_d[i][j]  = xtmp ;
        x_dc_ptr[i][j] = xtmp ;
      }
    }
  }
  /*---------------------------------------------------------------------*/
  /* main calculation                                                    */
  /*---------------------------------------------------------------------*/
  printf ("======================================================\n");
  printf ("Address mapping of 2D array in C                      \n");
  printf ("   Author:      Yukihiro Ota (yota@rist.or.jp)        \n");
  printf ("   Last update: 27 Oct 2017                           \n");
  printf ("======================================================\n");

  printf ("Static memory allocation in (double=8bytes)\n") ;
  printf ("x_s[i][j]\n") ;
  printf ("row    col    val         address (hex) \n") ;
  for ( int i=0; i<nrow; ++i ) {
    for ( int j=0; j<ncol; ++j ) {
      printf ("%5d  %5d  %11.4f  %p\n", i, j, x_s[i][j], &x_s[i][j] ) ;
    }
  }

  printf ("Typical dynamical memory allocation (double=8bytes)\n") ;
  printf ("x_d[i][j]\n") ;
  printf ("row    col    val         address (hex) \n") ;
  for ( int i=0; i<nrow; ++i ) {
    for ( int j=0; j<ncol; ++j ) {
      printf ("%5d  %5d  %11.4f  %p\n", i, j, x_d[i][j], &x_d[i][j] ) ;
    }
  }

  printf ("Alternative Dynamical memory allocation (double=8bytes)\n") ;
  printf ("x_dc_ptr[i][j]\n") ;
  printf ("row    col    val         address (hex) \n") ;
  for ( int i=0; i<nrow; ++i ) {
    for ( int j=0; j<ncol; ++j ) {
      printf ("%5d  %5d  %11.4f  %p\n", i, j, x_dc_ptr[i][j], 
              &x_dc_ptr[i][j] ) ;
    }
  }
  printf ("x_dc[k] with k = j + i*(num of col.)\n") ;
  printf ("row    col    val         address (hex) \n") ;
  for ( int i=0; i<nrow; ++i ) {
    for ( int j=0; j<ncol; ++j ) {
      int k = j + i*ncol ;
      printf ("%5d  %5d  %11.4f  %p\n", i, j, x_dc[k], &x_dc[k] ) ;
    }
  }


  /* Finalization */
  for ( int i=0; i<nrow; ++i ) {
    free ( x_d[i] ) ;
  }
  free ( x_d ) ; 

  free ( x_dc_ptr ) ; 
  free ( x_dc ) ;

  return EXIT_SUCCESS ;
}

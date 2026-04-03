// Copyright 2024 Research Organization for Information Science and Technology
//---------------------------------------------------------------------
//   Address mapping of 2D array in C++
//   Author:      Yukihiro Ota (yota@rist.or.jp)
//   Last update: 21st Jan., 2024
//----------------------------------------------------------------------
#include <cstdio>
#include <cstdlib>
#include <ctime>


template<typename T>
T *create_mem(T *&array, int n)
{
  int nbytes = sizeof(T) * n ;
  array = (T *)malloc(nbytes) ; 
  return array ; 
}


template<typename T>
void release_mem(T *&array) 
{
  free(array) ;
  array = nullptr;
}


template<typename T>
T **create_mem(T **&array, int n1, int n2)
{
  int nbytes = sizeof(T) * n1 * n2 ; // use of int64_t is preferable 
  T *data = (T *)malloc(nbytes) ;
  nbytes = sizeof(T *) * n1 ;
  array = (T **)malloc(nbytes) ;

  int n = 0 ;
  for ( int i = 0; i < n1; ++i ) {
    array[i] = &data[n] ;
    n += n2 ;
  }
  return array ;
}


template<typename T>
void release_mem(T **&array)
{
  if ( array == nullptr ) return ;
  free(array[0]) ;
  free(array) ;
  array = nullptr ;
}


int main ( int argc, char **argv )
{
  const int nrow = 3 ;  
  const int ncol = 8 ;  
  double   x_s[nrow][ncol] ; /* static        */
  double **x_d ;             /* dynamical     */
  int    **x_i ;             /* dynamical     */

  /* memory allocation */
  create_mem(x_d, nrow, ncol) ;
  create_mem(x_i, nrow, ncol) ;

  /*---------------------------------------------------------------------*/
  /* Initialization of data with random number                            */
  /*---------------------------------------------------------------------*/
  {
    srand( (unsigned int)time(NULL) ) ;
    /* in [0,1) */
    for ( int i=0; i<nrow; ++i ) {
    for ( int j=0; j<ncol; ++j ) {
      double xtmp = 2.0*(double )rand()/RAND_MAX - 1.0 ;
      x_s[i][j]  = xtmp ;
      x_d[i][j]  = xtmp ;
    }}
    /* in [0,4) */
    for ( int i=0; i<nrow; ++i ) {
    for ( int j=0; j<ncol; ++j ) {
      x_i[i][j]  = rand()%4 ;
    }}
  }
  /*---------------------------------------------------------------------*/
  /* main calculation                                                    */
  /*---------------------------------------------------------------------*/
  printf ("======================================================\n");
  printf ("Address mapping of 2D array in C++                    \n");
  printf ("   Author:      Yukihiro Ota (yota@rist.or.jp)        \n");
  printf ("   Last update: 23 Aug 2018                           \n");
  printf ("======================================================\n");

  printf ("Static memory allocation in (double=8bytes)\n") ;
  printf ("x_s[i][j]\n") ;
  printf ("row    col    val         address (hex) \n") ;
  for ( int i=0; i<nrow; ++i ) {
  for ( int j=0; j<ncol; ++j ) {
    printf ("%5d  %5d  %11.4f  %p\n", i, j, x_s[i][j], &x_s[i][j] ) ;
  }}

  printf ("Dynamical memory allocation (double=8bytes)\n") ;
  printf ("x_d[i][j]\n") ;
  printf ("row    col    val         address (hex) \n") ;
  for ( int i=0; i<nrow; ++i ) {
  for ( int j=0; j<ncol; ++j ) {
    printf ("%5d  %5d  %11.4f  %p\n", i, j, x_d[i][j], &x_d[i][j] ) ;
  }}

  printf ("Dynamical memory allocation (int=4bytes)\n") ;
  printf ("x_i[i][j]\n") ;
  printf ("row    col    val         address (hex) \n") ;
  for ( int i=0; i<nrow; ++i ) {
  for ( int j=0; j<ncol; ++j ) {
    printf ("%5d  %5d  %10d  %p\n", i, j, x_i[i][j], &x_i[i][j] ) ;
  }}

  /* finalize */
  release_mem(x_d) ;
  release_mem(x_i) ;

  return EXIT_SUCCESS ;
}

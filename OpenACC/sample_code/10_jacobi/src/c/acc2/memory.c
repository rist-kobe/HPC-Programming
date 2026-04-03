/* Copyright 2024 Research Organization for Information Science and Technology */
/*----------------------------------------------------------------------
 *   memory.c                                                          
 *   Author:      Yukihiro Ota (yota@rist.or.jp)
 *   Description: Dynamical memory allocation with contiguous access 
 *--------------------------------------------------------------------*/
#include "memory.h"
#include <stdlib.h>

/*---------------------------------------------------------------------
 * 1D integer array (vector)
 * [usage]
 *    To allocate 8-comp. vector: int *vec = create_int_1d_array(8);
 *    To access each element:     vec[i] = ...
 *                                (i=0, 1, ..., 7)
 *    To release its memory:      release_int_1d_array(vec);
 *--------------------------------------------------------------------*/
int *create_int_1d_array( const int n )
{  
   int nbytes = sizeof(int)*n;
   int *array = (int *)malloc(nbytes); 
   return array;
}
void release_int_1d_array( int *array)
{  
   free(array);
   array = NULL;
}
/*---------------------------------------------------------------------
 * 1D DP array (vector)
 * [usage]
 *    To allocate 8-comp. vector: double *vec = create_dp_1d_array(8);
 *    To access each element:     vec[i] = ...
 *                                (i=0, 1, ..., 7)
 *    To release its memory:      release_dp_1d_array(vec);
 *--------------------------------------------------------------------*/
double *create_dp_1d_array( const int n )
{  
   int nbytes = sizeof(double)*n;
   double *array = (double *)malloc(nbytes); 
   return array;
}
void release_dp_1d_array( double *array)
{  
   free(array);
   array = NULL;
}
/*---------------------------------------------------------------------
 * 2D integer array (matrix)
 * [usage]
 *    To allocate 4x7 matrix:     int **mat = create_int_2d_array(4,7);
 *    To access each element:     mat[i][j] = ...
 *                                (i=0, ..., 3; j=0, ..., 6)
 *    To release its memory:      release_int_2d_array(mat);
 *--------------------------------------------------------------------*/
int **create_int_2d_array( const int n1, const int n2)
{
   int nbytes = sizeof(int)*n1*n2; 
   int *data = (int *)malloc(nbytes);
   nbytes = sizeof(int *)*n1;
   int **array = (int **)malloc(nbytes);

   int i;
   int n = 0;
   for ( i = 0; i < n1; ++i ) {
      array[i] = &data[n];
      n += n2;
   }
   return array;
}
void release_int_2d_array( int **array)
{
   if ( array == NULL ) return;
   free(array[0]);
   free(array);
   array = NULL;
}
/*---------------------------------------------------------------------
 * 2D DP array (matrix)
 * [usage]
 *    To allocate 4x7 matrix:     double **mat = create_dp_2d_array(4,7);
 *    To access each element:     mat[i][j] = ...
 *                                (i=0, ..., 3; j=0, ..., 6)
 *    To release its memory:      release_dp_2d_array(mat);
 *--------------------------------------------------------------------*/
double **create_dp_2d_array( const int n1, const int n2)
{
   int nbytes = sizeof(double)*n1*n2; 
   double *data = (double *)malloc(nbytes);
   nbytes = sizeof(double *)*n1;
   double **array = (double **)malloc(nbytes);

   int i;
   int n = 0;
   for ( i = 0; i < n1; ++i ) {
      array[i] = &data[n];
      n += n2;
   }
   return array;
}
void release_dp_2d_array( double **array)
{
   if ( array == NULL ) return;
   free(array[0]);
   free(array);
   array = NULL;
}
/*---------------------------------------------------------------------
 * 3D integer array 
 * [usage]
 *    To allocate 4x7x3 array:    int ***a = create_int_3d_array(4,7,3);
 *    To access each element:     a[i][j][k] = ...
 *                                (i=0, ..., 3; j=0, ..., 6; k=0,1,2)
 *    To release its memory:      release_int_3d_array(a);
 *--------------------------------------------------------------------*/
int ***create_int_3d_array( const int n1, const int n2, const int n3)
{
   int nbytes = sizeof(int)*n1*n2*n3; /* 64-bit unsigned int should be used */ 
   int *data = (int *)malloc(nbytes);
   nbytes = sizeof(int *)*n1*n2;
   int **array2 = (int **)malloc(nbytes);
   nbytes = sizeof(int **)*n1;
   int ***array = (int ***)malloc(nbytes);

   int i, n;

   n = 0;
   for ( i = 0; i < n1*n2; ++i ){
      array2[i] = &data[n];
      n += n3; 
   }

   n = 0;
   for ( i = 0; i < n1; ++i ){
      array[i] = &array2[n];
      n += n2; 
   }
   return array;
}
void release_int_3d_array( int ***array)
{
   if ( array == NULL ) return;
   free(array[0][0]);
   free(array[0]);
   free(array);
   array = NULL;
}
/*---------------------------------------------------------------------
 * 3D DP array 
 * [usage]
 *    To allocate 4x7x3 array:    double ***a = create_dp_3d_array(4,7,3);
 *    To access each element:     a[i][j][k] = ...
 *                                (i=0, ..., 3; j=0, ..., 6; k=0,1,2)
 *    To release its memory:      release_dp_3d_array(a);
 *--------------------------------------------------------------------*/
double ***create_dp_3d_array( const int n1, const int n2, const int n3)
{
   int nbytes = sizeof(double)*n1*n2*n3; /* 64-bit unsigned int should be used */ 
   double *data = (double *)malloc(nbytes);
   nbytes = sizeof(double *)*n1*n2;
   double **array2 = (double **)malloc(nbytes);
   nbytes = sizeof(double **)*n1;
   double ***array = (double ***)malloc(nbytes);

   int i, n;

   n = 0;
   for ( i = 0; i < n1*n2; ++i ){
      array2[i] = &data[n];
      n += n3; 
   }

   n = 0;
   for ( i = 0; i < n1; ++i ){
      array[i] = &array2[n];
      n += n2; 
   }
   return array;
}
void release_dp_3d_array( double ***array)
{
   if ( array == NULL ) return;
   free(array[0][0]);
   free(array[0]);
   free(array);
   array = NULL;
}

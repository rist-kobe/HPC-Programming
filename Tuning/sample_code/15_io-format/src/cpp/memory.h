// Copyright 2024 Research Organization for Information Science and Technology 
#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <cstdlib>
/*---------------------------------------------------------------------
 * 1D array (vector)
 * [usage]
 *    To allocate 8-comp. vector: int *vec; create_1d_array(vec, 8);
 *    To access each element:     vec[i] = ...
 *                                (i=0, 1, ..., 7)
 *    To release its memory:      release_1d_array(vec);
 *--------------------------------------------------------------------*/
template<typename T>
T *create_1d_array( T *&array, const int n )
{  
   std::size_t nbytes = sizeof(T)*(std::size_t)n;
   array = (T *)malloc(nbytes); 
   return array;
}
template<typename T>
void release_1d_array( T *&array)
{  
   free(array);
#if ! defined(__NOT_STDCPP11__)
   array = nullptr;
#else
   array = NULL;
#endif
}
/*---------------------------------------------------------------------
 * 2D array (matrix)
 * [usage]
 *    To allocate 4x7 matrix:     int **mat; create_2d_array(mat,4,7);
 *    To access each element:     mat[i][j] = ...
 *                                (i=0, ..., 3; j=0, ..., 6)
 *    To release its memory:      release_2d_array(mat);
 *--------------------------------------------------------------------*/
template<typename T>
T **create_2d_array( T **&array, const int n1, const int n2)
{
   std::size_t nbytes = sizeof(T)*(std::size_t)n1*(std::size_t)n2; 
   T *data = (T *)malloc(nbytes);
   nbytes = sizeof(T *)*(std::size_t)n1;
   array = (T **)malloc(nbytes);

   int i;
   std::size_t n = 0;
   for ( i = 0; i < n1; ++i ) {
      array[i] = &data[n];
      n += n2;
   }
   return array;
}
template<typename T>
void release_2d_array( T **&array)
{
#if ! defined(__NOT_STDCPP11__)
   if ( array == nullptr ) return;
   free(array[0]);
   free(array);
   array = nullptr;
#else
   if ( array == NULL ) return;
   free(array[0]);
   free(array);
   array = NULL;
#endif
}
/*---------------------------------------------------------------------
 * 3D array 
 * [usage]
 *    To allocate 4x7x3 array:    int ***a; create_3d_array(a,4,7,3);
 *    To access each element:     a[i][j][k] = ...
 *                                (i=0, ..., 3; j=0, ..., 6; k=0,1,2)
 *    To release its memory:      release_3d_array(a);
 *--------------------------------------------------------------------*/
template<typename T>
T ***create_3d_array( T ***&array, const int n1, const int n2, const int n3)
{
   std::size_t nbytes = sizeof(T)*(std::size_t)n1*(std::size_t)n2*(std::size_t)n3;
   T *data = (T *)malloc(nbytes);
   nbytes = sizeof(T *)*(std::size_t)n1*(std::size_t)n2;
   T **array2 = (T **)malloc(nbytes);
   nbytes = sizeof(T **)*(std::size_t)n1;
   array = (T ***)malloc(nbytes);

   int i;
   std::size_t n;

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
template<typename T>
void release_3d_array( T ***&array )
{
#if ! defined(__NOT_STDCPP11__)
   if ( array == nullptr ) return;
   free(array[0][0]);
   free(array[0]);
   free(array);
   array = nullptr;
#else
   if ( array == NULL ) return;
   free(array[0][0]);
   free(array[0]);
   free(array);
   array = NULL;
#endif
}
#endif /* __MEMORY_H__ */

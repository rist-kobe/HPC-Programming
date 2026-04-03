/* Copyright 2024 Research Organization for Information Science and Technology */
#ifndef __MEMORY_H__
#define __MEMORY_H__

int *create_int_1d_array( const int );
void release_int_1d_array( int *);
double *create_dp_1d_array( const int );
void release_dp_1d_array( double *);
int **create_int_2d_array( const int, const int);
void release_int_2d_array( int **);
double **create_dp_2d_array( const int, const int);
void release_dp_2d_array( double **);
int ***create_int_3d_array( const int, const int, const int);
void release_int_3d_array( int ***);
double ***create_dp_3d_array( const int, const int, const int);
void release_dp_3d_array( double ***);

#endif /* __MEMORY_H__ */

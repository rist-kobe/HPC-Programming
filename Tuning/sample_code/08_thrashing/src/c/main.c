/* Copyright 2024 Research Organization for Information Science and Technology */
/*--------------------------------------------------------------------
  Thrashing
  Author:       Yukihiro Ota (yota@rist.or.jp)
  Last update:  23rd Jan., 2024
  Usage:        run.x (arg1)
                  (arg1) = size of vectors (integer) = nsize
                When nsize = (integer)*(Size of L1 D cache per way)/8bytes,
                thrasing may occur. Examine elapsed time around 
                this target value of nsize.
  Note 1:       Before running this, check size of L1 data cache per 
                way in your computer. On Unix/Linux, use of getconf
                command is a practical option for this purpose.
  Note 2:       If cache associativity (i.e., number of ways) is 
                larger, an effect of thrashing will be less.  
----------------------------------------------------------------------*/
#include "mykernel.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/*--------------------------------------------------------------------
   Memory management
----------------------------------------------------------------------*/
double *create_vec_d(int n)
{
   int nbytes = sizeof(double) * n ;
   double *a = (double *)malloc( nbytes ) ;
   return a ;
}

void release_vec_d( double *a )
{
   free( a ) ; a = NULL ;
}
/*--------------------------------------------------------------------
   Timer routine 
----------------------------------------------------------------------*/
#if ! defined(NOT_USE_REALTIME)
double get_elp_time () {
  struct timespec tp ;
  clock_gettime ( CLOCK_REALTIME, &tp ) ;
  return  tp.tv_sec + (double)tp.tv_nsec*1.0e-9 ;
}
#else
double get_elp_time () {
  struct timespec tp ;
  clock_gettime ( CLOCK_MONOTONIC, &tp ) ;
  return  tp.tv_sec + (double)tp.tv_nsec*1.0e-9 ;
}
#endif

double get_cpu_time () {
  struct timespec tp ;
  clock_gettime ( CLOCK_PROCESS_CPUTIME_ID, &tp ) ;
  return  tp.tv_sec + (double)tp.tv_nsec*1.0e-9 ;
}
/*--------------------------------------------------------------------
   Main function 
----------------------------------------------------------------------*/
int main( int argc, char **argv) 
{
  /* parse command-line options                                       */
  if ( argc != 2 ) {
    printf("[usage] run.x (arg1) \n") ;
    printf("  arg1: vector size (integer)\n") ;
    return EXIT_SUCCESS ;
  }

  int nsize = atoi(argv[1]) ;

  /* memory allocation                                                */
  double *u;
  double *a;
  double *b;
  double *c;
  double *d;
  double *e;
  double *f;
  double *g;
  double *h;

  u = create_vec_d( nsize );
  a = create_vec_d( nsize );
  b = create_vec_d( nsize );
  c = create_vec_d( nsize );
  d = create_vec_d( nsize );
  e = create_vec_d( nsize );
  f = create_vec_d( nsize );
  g = create_vec_d( nsize );
  h = create_vec_d( nsize );

  for ( int i = 0 ; i < nsize; ++i ) {
     u[i] = 0.0;
     a[i] = 1.0e-2;
     b[i] = 2.0e-2;
     c[i] = 3.0e-2;
     d[i] = 4.0e-2;
     e[i] = 1.0e-2;
     f[i] = 2.0e-2;
     g[i] = 3.0e-2;
     h[i] = 4.0e-2;
  }

  double memuse = 0.0 ;
  memuse += sizeof(double) * nsize ;
  memuse /= 1024 ; /* KiB */

  /* main calculation                                                 */
  double cpu1, cpu21, cpu22;
  int icon, nitr;

  nitr = 10000;
  icon = 0;
  cpu1 = get_cpu_time() ; 
  /* 4 loads, 1 load and store */
  for ( int itr = 0 ; itr <= nitr; ++itr ) {
    kernel_l4_ls1(icon, nsize, a, b, c, d, u);
    icon = itr % 2;
  }
  cpu21 = get_cpu_time() - cpu1;

  for ( int i = 0 ; i < nsize; ++i ) {
     u[i] = 0.0 ;
  }

  nitr = 10000;
  icon = 0;
  cpu1 = get_cpu_time() ; 
  /* 4 loads, 1 load and store */
  for ( int itr = 0 ; itr <= nitr; ++itr ) {
    kernel_l8_ls1(icon, nsize, a, b, c, d, e, f, g, h, u);
    icon = itr % 2;
  }
  cpu22 = get_cpu_time() - cpu1;

  /* output                                                           */
  printf("nsize= %d\n", nsize);
  printf("# kernel\n");
  printf("kernel_l4_ls1: 4 loads, 1 load and store.\n");
  printf("  Thrashing may occur when associativity is less than and equal to 4.\n");
  printf("kernel_l8_ls1: 8 loads, 1 load and store.\n");
  printf("  Thrashing may occur when associativity is less than and equal to 8.\n");
  printf("# output\n");
  printf("col 1: size of array (8bytes)\n"); 
  printf("col 2: memory per array (KiB)\n"); 
  printf("col 3: cpu time of kernel_l4_ls1 (sec.)\n"); 
  printf("col 4: cpu time of kernel_l8_ls1 (sec.)\n"); 
  printf("col 5: number of arrays in kernel_l4_ls1\n"); 
  printf("col 6: number of arrays in kernel_l8_ls1\n"); 
  printf("col 7: dummy output\n"); 
  printf("-------------------------\n");

  FILE *fp;
  fp = fopen("logfile","a");
  fprintf(fp, "%d %10.4f %9.3f %9.3f %d %d %10.4f\n",nsize,memuse,cpu21,cpu22,5,9,u[0]);
  fclose(fp);

  fp = fopen("add.info","a") ;
  fprintf(fp,"nsize = %d\n", nsize);
  fprintf(fp,"%p %p\n", &u[0],&u[nsize-1]);
  fprintf(fp,"%p %p\n", &a[0],&a[nsize-1]);
  fprintf(fp,"%p %p\n", &b[0],&b[nsize-1]);
  fprintf(fp,"%p %p\n", &c[0],&c[nsize-1]);
  fprintf(fp,"%p %p\n", &d[0],&d[nsize-1]);
  fprintf(fp,"%p %p\n", &e[0],&e[nsize-1]);
  fprintf(fp,"%p %p\n", &f[0],&f[nsize-1]);
  fprintf(fp,"%p %p\n", &g[0],&g[nsize-1]);
  fprintf(fp,"%p %p\n", &h[0],&h[nsize-1]);
  fprintf(fp,"-------------------------\n");
  fclose(fp) ;
 
  /* finalization */
  release_vec_d( u );
  release_vec_d( a );
  release_vec_d( b );
  release_vec_d( c );
  release_vec_d( d );
  release_vec_d( e );
  release_vec_d( f );
  release_vec_d( g );
  release_vec_d( h );

  return EXIT_SUCCESS ;
}

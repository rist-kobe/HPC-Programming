/* Copyright 2024 Research Organization for Information Science and Technology */
#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "timer.h"

int main(int argc, char **argv)
{
  const int nx=1024; /* 2^10 */
  const int ny=1024; /* 2^10 */
  int nz;
  int i, j, k;
  int ifmt;
  int aasize;

  FILE *fp;

  double elp, cpu, elp0;
  double aamemsize;

  if ( argc != 3 ) {
     fprintf(stdout,"[usage] ./run.x (arg1) (arg2)\n");
     fprintf(stdout,"   (arg1): Integer (0-1)\n");
     fprintf(stdout,"           0: write w/  format \n");
     fprintf(stdout,"           1: write w/o format \n");
     fprintf(stdout,"   (arg2): Integer (<200) to change array size\n");
     return EXIT_SUCCESS;
  }

  ifmt = atoi(argv[1]);
  nz = atoi(argv[2]);

  if ( nz > 200 ) {
     fprintf(stdout,"[Error] 2nd arg must be < 200.");
     return EXIT_FAILURE;
  }

  int ***aa = create_int_3d_array( nx, ny, nz);

  for ( i=0; i<nx; ++i ) {
  for ( j=0; j<ny; ++j ) {
  for ( k=0; k<nz; ++k ) {
      aa[i][j][k] = k + (j + i*ny)*nz;
  }}}

  aasize = nx*ny*nz;
  aamemsize = 4.0*nx*ny*nz / 1048576; /* MiB */

  /*-----------------------------------------------------------------
   * write with format
   *----------------------------------------------------------------*/
  if ( ifmt == 0 ) {

    fp = fopen("data1.out","w");
    elp0 = get_elp_time();
    for ( i=0; i<nx; ++i ) {
    for ( j=0; j<ny; ++j ) {
    for ( k=0; k<nz; ++k ) {
       fprintf(fp,"%d\n",aa[i][j][k]);
    }}}
    elp = get_elp_time() - elp0;
    fclose(fp);

    fp = fopen("data2.out","w");
    elp0 = get_cpu_time();
    for ( i=0; i<nx; ++i ) {
    for ( j=0; j<ny; ++j ) {
    for ( k=0; k<nz; ++k ) {
       fprintf(fp,"%d\n",aa[i][j][k]);
    }}}
    cpu = get_cpu_time() - elp0;
    fclose(fp);

  /*-----------------------------------------------------------------
   * write without format
   *----------------------------------------------------------------*/
  } else if ( ifmt == 1 ) {

    fp = fopen("data1.out","wb");
    elp0 = get_elp_time();
    fwrite(&aa[0][0][0],sizeof(int),aasize,fp);
    elp = get_elp_time() - elp0;
    fclose(fp);

    fp = fopen("data2.out","wb");
    elp0 = get_cpu_time();
    fwrite(&aa[0][0][0],sizeof(int),aasize,fp);
    cpu = get_cpu_time() - elp0;
    fclose(fp);

  } else {
    fprintf(stdout,"[Error] No routine. Check command-line options\n");
    release_int_3d_array(aa);
    return EXIT_FAILURE;
  }

  fprintf(stdout,"format memory_MiB elapsed_sec cpu_sec diff\n");
  fprintf(stdout,"%d %11.4f %11.4f %11.4f %11.4f\n",
    ifmt, aamemsize, elp, cpu, elp-cpu);

  release_int_3d_array(aa);

  return EXIT_SUCCESS;
}

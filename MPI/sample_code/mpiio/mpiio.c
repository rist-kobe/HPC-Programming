#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <sys/syscall.h>
#include <omp.h>
#include "mpi.h"
int main(int argc,char *argv[]){
  int sizeW,rankW;
  char nameF0[256];
  FILE *fp0;
  MPI_Init(NULL,NULL);
  MPI_Comm_size(MPI_COMM_WORLD,&sizeW);
  MPI_Comm_rank(MPI_COMM_WORLD,&rankW);
  sprintf(nameF0,"%s_%d.txt",argv[0],rankW);
  fp0=fopen(nameF0,"w");
  fprintf(fp0," rankW=%2d sizeW=%2d\n",rankW,sizeW);
  int nd;
  int *sendbuf;
  nd=1;
  sendbuf=(int *)malloc(sizeof(int)*nd);
  sendbuf[0]=rankW*10;
  char nameF1[256];
  sprintf(nameF1,"%s_m.dat",argv[0]);
  MPI_File fp1;
  MPI_File_open(MPI_COMM_WORLD,nameF1,MPI_MODE_WRONLY|MPI_MODE_CREATE,MPI_INFO_NULL,&fp1);
  MPI_File_write_at(fp1,rankW*sizeof(int)*nd,sendbuf,nd,MPI_INT,MPI_STATUS_IGNORE);
  MPI_File_close(&fp1);
  free(sendbuf);
  fclose(fp0);
  MPI_Finalize();
  return 0;
}

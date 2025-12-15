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
  double wtick,time1,time2;
  wtick=MPI_Wtick();
  time1=MPI_Wtime();
  sleep(rankW);
  MPI_Barrier(MPI_COMM_WORLD);
  time2=MPI_Wtime();
  fprintf(fp0," %4.2e %4.2e\n",time2-time1,wtick);
  fclose(fp0);
  MPI_Finalize();
  return 0;
}

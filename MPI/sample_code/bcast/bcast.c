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
  int nd,rootW,i;
  int *bcstbuf;
  nd=1;
  bcstbuf=(int *)malloc(sizeof(int)*nd);
  bcstbuf[0]=rankW*10;
  fprintf(fp0,"\n");
  for(i=0;i<nd;i++){
    fprintf(fp0," index=%2d  bcst=%02d\n",i,bcstbuf[i]);
  }
  rootW=0;
  MPI_Bcast(bcstbuf,nd,MPI_INT,rootW,MPI_COMM_WORLD);
  fprintf(fp0,"\n");
  for(i=0;i<nd;i++){
    fprintf(fp0," index=%2d  bcst=%02d\n",i,bcstbuf[i]);
  }
  free(bcstbuf);
  fclose(fp0);
  MPI_Finalize();
  return 0;
}

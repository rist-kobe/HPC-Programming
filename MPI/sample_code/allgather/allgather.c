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
  int nd,i;
  int *sendbuf,*recvbuf;
  nd=1;
  sendbuf=(int *)malloc(sizeof(int)*nd);
  recvbuf=(int *)malloc(sizeof(int)*nd*sizeW);
  sendbuf[0]=rankW*10;
  for(i=0;i<sizeW;i++){
    recvbuf[i]=-1;
  }
  fprintf(fp0,"\n");
  for(i=0;i<nd;i++){
    fprintf(fp0," index=%2d  send=%02d\n",i,sendbuf[i]);
  }
  MPI_Allgather(sendbuf,nd,MPI_INT,recvbuf,nd,MPI_INT,MPI_COMM_WORLD);
  fprintf(fp0,"\n");
  for(i=0;i<nd*sizeW;i++){
    fprintf(fp0," index=%2d  recv=%02d\n",i,recvbuf[i]);
  }
  free(recvbuf);
  free(sendbuf);
  fclose(fp0);
  MPI_Finalize();
  return 0;
}

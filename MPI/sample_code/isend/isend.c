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
  int *sendbuf,*recvbuf;
  nd=1;
  sendbuf=(int *)malloc(sizeof(int)*nd);
  recvbuf=(int *)malloc(sizeof(int)*nd);
  sendbuf[0]=rankW*10;
  recvbuf[0]=-1; 
  MPI_Request request[2];
  int tag=32767;
  if(rankW==0){
    MPI_Irecv(recvbuf,nd,MPI_INT,sizeW-1,tag,MPI_COMM_WORLD,&(request[0]));
    MPI_Isend(sendbuf,nd,MPI_INT,rankW+1,tag,MPI_COMM_WORLD,&(request[1]));
  }else if(0<rankW&&rankW<sizeW-1){ 
    MPI_Irecv(recvbuf,nd,MPI_INT,rankW-1,tag,MPI_COMM_WORLD,&(request[0]));
    MPI_Isend(sendbuf,nd,MPI_INT,rankW+1,tag,MPI_COMM_WORLD,&(request[1]));
  }else{
    MPI_Irecv(recvbuf,nd,MPI_INT,rankW-1,tag,MPI_COMM_WORLD,&(request[0]));
    MPI_Isend(sendbuf,nd,MPI_INT,0,tag,MPI_COMM_WORLD,&(request[1]));
  }
  MPI_Waitall(2,request,MPI_STATUSES_IGNORE);
  fprintf(fp0," recvW=%02d sendW=%02d\n",recvbuf[0],sendbuf[0]);
  free(recvbuf);
  free(sendbuf);
  fclose(fp0);
  MPI_Finalize();
  return 0;
}

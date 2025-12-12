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
  int ndims,reorder,idim,shft,rank_src,rank_dst,nd;
  int *dims,*prds,*sendbuf,*recvbuf;
  ndims=1;
  dims=(int *)malloc(sizeof(int)*ndims);
  prds=(int *)malloc(sizeof(int)*ndims);
  dims[0]=sizeW;
  prds[0]=true;
  reorder=false;
  MPI_Comm mpi_comm_cart;
  MPI_Cart_create(MPI_COMM_WORLD,ndims,dims,prds,reorder,&mpi_comm_cart);
  idim=0;
  shft=1;
  MPI_Cart_shift(mpi_comm_cart,idim,shft,&rank_src,&rank_dst);
  nd=1;
  sendbuf=(int *)malloc(sizeof(int)*nd);
  recvbuf=(int *)malloc(sizeof(int)*nd);
  sendbuf[0]=rankW*10; 
  recvbuf[0]=-1; 
  MPI_Request request[2];
  int tag=32767;
  MPI_Irecv(recvbuf,nd,MPI_INT,rank_src,tag,MPI_COMM_WORLD,&(request[0]));
  MPI_Isend(sendbuf,nd,MPI_INT,rank_dst,tag,MPI_COMM_WORLD,&(request[1]));
  MPI_Waitall(2,request,MPI_STATUSES_IGNORE);
  fprintf(fp0," recvW=%02d sendW=%02d\n",recvbuf[0],sendbuf[0]);
  free(recvbuf);
  free(sendbuf);
  free(prds);
  free(dims);
  fclose(fp0);
  MPI_Finalize();
  return 0;
}

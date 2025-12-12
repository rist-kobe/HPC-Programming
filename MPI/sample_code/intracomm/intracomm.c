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
  int sizeS,rankS,nd,rootS,i;
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
  MPI_Group mpi_group_world,group1,group2;
  MPI_Comm mpi_comm_group1,mpi_comm_group2;
  MPI_Comm_group(MPI_COMM_WORLD,&mpi_group_world);
  int ranks1[2]={0,1};
  int ranks2[2]={2,3};
  MPI_Group_incl(mpi_group_world,2,ranks1,&group1);
  MPI_Group_incl(mpi_group_world,2,ranks2,&group2);
  MPI_Comm_create(MPI_COMM_WORLD,group1,&mpi_comm_group1);
  MPI_Comm_create(MPI_COMM_WORLD,group2,&mpi_comm_group2);
  rootS=1;
  if(mpi_comm_group1!=MPI_COMM_NULL){
    MPI_Comm_size(mpi_comm_group1,&sizeS);
    MPI_Comm_rank(mpi_comm_group1,&rankS);
    fprintf(fp0,"\n");
    fprintf(fp0," rankS=%2d sizeS=%2d\n",rankS,sizeS);
    MPI_Gather(sendbuf,nd,MPI_INT,recvbuf,nd,MPI_INT,rootS,mpi_comm_group1);
  }
  if(mpi_comm_group2!=MPI_COMM_NULL){
    MPI_Comm_size(mpi_comm_group2,&sizeS);
    MPI_Comm_rank(mpi_comm_group2,&rankS);
    fprintf(fp0,"\n");
    fprintf(fp0," rankS=%2d sizeS=%2d\n",rankS,sizeS);
    MPI_Gather(sendbuf,nd,MPI_INT,recvbuf,nd,MPI_INT,rootS,mpi_comm_group2);
  }
  fprintf(fp0,"\n");
  for(i=0;i<sizeW;i++){
    fprintf(fp0," index=%2d  recv=%02d\n",i,recvbuf[i]);
  }
  free(recvbuf);
  free(sendbuf);
  fclose(fp0);
  MPI_Finalize();
  return 0;
}

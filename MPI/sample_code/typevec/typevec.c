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
  int nd,tag,i,bcount,blklen,stride;
  int *sendbuf,*recvbuf;
  nd=4;
  sendbuf=(int *)malloc(sizeof(int)*nd);
  recvbuf=(int *)malloc(sizeof(int)*nd);
  for(i=0;i<nd;i++){
    sendbuf[i]=rankW*10+i;
    recvbuf[i]=-1;
  }
  fprintf(fp0,"\n");
  for(i=0;i<nd;i++){
    fprintf(fp0," index=%2d  send=%02d\n",i,sendbuf[i]);
  }
  bcount=2;
  blklen=1;
  stride=2;
  MPI_Datatype newtype;
  MPI_Type_vector(bcount,blklen,stride,MPI_INT,&newtype);
  MPI_Type_commit(&newtype);
  tag=32767;
  if(rankW==0){
    MPI_Send(&(sendbuf[1]),1,newtype,sizeW-1,tag,MPI_COMM_WORLD);
  }
  if(rankW==sizeW-1){
    MPI_Recv(&(recvbuf[1]),1,newtype,0,tag,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
  }
  MPI_Type_free(&newtype);
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

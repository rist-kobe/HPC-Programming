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
  int nd,tag,nl,i;
  int *sendbuf,*recvbuf;
  time_t time_1,time_2;
  nd=1;
  sendbuf=(int *)malloc(sizeof(int)*nd);
  recvbuf=(int *)malloc(sizeof(int)*nd);
  sendbuf[0]=-1; 
  recvbuf[0]=-1; 
  tag=32767;
  nl=7;
  time_1=time(NULL);
  if(rankW==0){
    FILE *fp1;
    fp1=fopen("send_input.txt","r");
    for(i=0;i<nl;i++){
      fscanf(fp1,"%d",&(sendbuf[0]));
      sleep(2);
      time_2=time(NULL);
      fprintf(fp0,"second=%2d sendW=%2d\n",(int)(time_2-time_1),sendbuf[0]);
      MPI_Send(sendbuf,nd,MPI_INT,rankW+1,tag,MPI_COMM_WORLD);
    }
    fclose(fp1);
  }else if(0<rankW&&rankW<sizeW-1){ 
    for(i=0;i<nl;i++){
      MPI_Recv(recvbuf,nd,MPI_INT,rankW-1,tag,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      sendbuf[0]=recvbuf[0]*(rankW+1);
      sleep(2);
      time_2=time(NULL);
      fprintf(fp0,"second=%2d sendW=%2d\n",(int)(time_2-time_1),sendbuf[0]);
      MPI_Send(sendbuf,nd,MPI_INT,rankW+1,tag,MPI_COMM_WORLD);
    }
  }else{
    FILE *fp1;
    fp1=fopen("send_output.txt","w");
    for(i=0;i<nl;i++){
      MPI_Recv(recvbuf,nd,MPI_INT,rankW-1,tag,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      fprintf(fp1,"%2d\n",recvbuf[0]);
      sleep(2);
      time_2=time(NULL);
      fprintf(fp0,"second=%2d recvW=%2d\n",(int)(time_2-time_1),recvbuf[0]);
    }
    fclose(fp1);
  }
  free(recvbuf);
  free(sendbuf);
  fclose(fp0);
  MPI_Finalize();
  return 0;
}

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
  recvbuf=(int *)malloc(sizeof(int)*nd*3);
  sendbuf[0]=rankW*10;
  for(int i=0;i<3;i++){
    recvbuf[i]=-1;
  }
  fprintf(fp0,"\n");
  for(int i=0;i<nd;i++){
    fprintf(fp0," index=%2d  send=%02d\n",i,sendbuf[i]);
  }
  int ranks[3];
  ranks[0]=rank_src;
  ranks[1]=rankW;
  ranks[2]=rank_dst;
  int (*ranks_tmp)[3]=(int (*)[3])malloc(sizeof(int[3])*sizeW);
  MPI_Allgather(ranks,3,MPI_INT,ranks_tmp,3,MPI_INT,MPI_COMM_WORLD);
  MPI_Comm *mpi_comm_sub=(MPI_Comm *)malloc(sizeof(MPI_Comm)*sizeW);
  MPI_Group mpi_group_world,mpi_group_sub;
  MPI_Comm_group(MPI_COMM_WORLD,&mpi_group_world);
  for(int i=0;i<sizeW;i++){
    MPI_Group_incl(mpi_group_world,3,&(ranks_tmp[i][0]),&mpi_group_sub);
    MPI_Comm_create(MPI_COMM_WORLD,mpi_group_sub,&(mpi_comm_sub[i]));
  }
  MPI_Request requests[3];
  MPI_Igather(sendbuf,nd,MPI_INT,recvbuf,nd,MPI_INT,1,mpi_comm_sub[ranks[0]],&(requests[0]));
  MPI_Igather(sendbuf,nd,MPI_INT,recvbuf,nd,MPI_INT,1,mpi_comm_sub[ranks[1]],&(requests[1]));
  MPI_Igather(sendbuf,nd,MPI_INT,recvbuf,nd,MPI_INT,1,mpi_comm_sub[ranks[2]],&(requests[2]));
  MPI_Waitall(3,requests,MPI_STATUSES_IGNORE);
  fprintf(fp0,"\n");
  for(int i=0;i<nd*3;i++){
    fprintf(fp0," index=%2d  recv=%02d\n",i,recvbuf[i]);
  }
  free(mpi_comm_sub);
  free(ranks_tmp);
  free(recvbuf);
  free(sendbuf);
  free(prds);
  free(dims);
  fclose(fp0);
  MPI_Finalize();
  return 0;
}

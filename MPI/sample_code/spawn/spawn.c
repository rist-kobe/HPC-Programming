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
  int procid;
  unsigned int coreid,numaid;
  char nameH[9]="@@@@@@@@";
  gethostname(nameH,8);nameH[8]='\0';
  procid=(int)getpid();
  syscall(__NR_getcpu,&coreid,&numaid,NULL);
  fprintf(fp0," hostname:%8s\n procid  :%8d\n",nameH,procid);
  fprintf(fp0," numaid  :%8u\n coreid  :%8u\n",numaid,coreid);
  MPI_Comm mpi_comm_child;
  int rankS=sizeW-1;
  char *command="bash";
  char *arguments[]={"numa_bind_exec.sh","2","../world","world",NULL};
  MPI_Comm_spawn(command,arguments,4,MPI_INFO_NULL,
                 rankS,MPI_COMM_WORLD,&mpi_comm_child,MPI_ERRCODES_IGNORE);
  fclose(fp0);
  MPI_Finalize();
  return 0;
}

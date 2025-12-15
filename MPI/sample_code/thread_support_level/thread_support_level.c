#include<stdio.h>
#include"mpi.h"
int main(){
  int provided; 
  MPI_Init_thread(NULL,NULL,MPI_THREAD_MULTIPLE,&provided);
  switch(provided){
    case MPI_THREAD_SINGLE:
      printf("MPI_THREAD_SINGLE\n");  
      break;
    case MPI_THREAD_FUNNELED:
      printf("MPI_THREAD_FUNNELED\n");  
      break;
    case MPI_THREAD_SERIALIZED:
      printf("MPI_THREAD_SERIALIZED\n");  
      break;
    default:
      printf("MPI_THREAD_MULTIPLE\n");  
  }
  MPI_Finalize();
  return 0;
}

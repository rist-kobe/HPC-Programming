/* Copyright 2024 Research Organization for Information Science and Technology */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  int tid0,tid1,tid2,tid3;
  #pragma omp parallel private(tid0)
  {
    #pragma omp single 
    {
      tid0 = omp_get_thread_num();   
      printf("I am %d. Hello world from a random thread.\n",tid0);
      #pragma omp task private(tid1)
      {
        tid1 = omp_get_thread_num();
        printf("task 1 run by tid=%d, queued by %d\n",tid1,tid0);
      }
      #pragma omp task private(tid2)
      {
        tid2 = omp_get_thread_num();
        printf("task 2 run by tid=%d, queued by %d\n",tid2,tid0);
      }
      #pragma omp task private(tid3)
      {
        tid3 = omp_get_thread_num();
        printf("task 3 run by tid=%d, queued by %d\n",tid3,tid0);
      }
    } /* End of omp single */
  } /* End of omp parallel */
  return EXIT_SUCCESS;
}

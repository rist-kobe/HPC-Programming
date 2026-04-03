/* Copyright 2024 Research Organization for Information Science and Technology */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void funcA();
void funcB();
void funcC();
void funcD();

int main(int argc, char** argv)
{
  #pragma omp parallel
  {
    #pragma omp sections
    {
      #pragma omp section 
      funcA(); 
      #pragma omp section 
      funcB(); 
      #pragma omp section 
      funcC(); 
      #pragma omp section 
      funcD(); 
    } /* End of omp sections */
  } /* End of omp parallel */
  return EXIT_SUCCESS;
}

void funcA()
{
   int ith = omp_get_thread_num();
   printf("Hello. I am funcA, with thread id %d\n",ith);
}

void funcB()
{
   int ith = omp_get_thread_num();
   printf("Hello. I am funcB, with thread id %d\n",ith);
}

void funcC()
{
   int ith = omp_get_thread_num();
   printf("Hello. I am funcC, with thread id %d\n",ith);
}

void funcD()
{
   int ith = omp_get_thread_num();
   printf("Hello. I am funcD, with thread id %d\n",ith);
}

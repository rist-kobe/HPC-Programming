/* Copyright 2024 Research Organization for Information Science and Technology */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void func(const char *name);

int main(void)
{
  #pragma omp parallel
  {
    /* Each section is executed exactly once by one thread of the
     * team. There is no ordering guarantee among the sections, and
     * an implied barrier exists at the end of the sections construct
     * (unless a nowait clause is added).                            */
    #pragma omp sections
    {
      #pragma omp section
      func("funcA");
      #pragma omp section
      func("funcB");
      #pragma omp section
      func("funcC");
      #pragma omp section
      func("funcD");
    } /* End of omp sections */
  } /* End of omp parallel */
  return EXIT_SUCCESS;
}

void func(const char *name)
{
   int ith = omp_get_thread_num();
   printf("Hello. I am %s, with thread id %d\n", name, ith);
}

// Copyright 2025 Research Organization for Information Science and Technology
// Reference
// * Tom Deakin and Timothy G. Mattson, 
//   "Programming your GPU with OpenMP: Performance portability for GPUs"
//   (MIT, 2023) Chap.8 and Appendix.
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <omp.h>

#define PRINTAPI(x) printf("%s=%d\n", #x, x)

int main (int argc, char **argv)
{
   fprintf(stdout,"%s Starting ... \n", argv[0]);
   PRINTAPI(_OPENMP);

   int deviceCount = omp_get_num_devices();

   if ( deviceCount == 0 ) {
      fprintf(stdout,"There is no available device.\n");
      return EXIT_SUCCESS;
   } else {
      fprintf(stdout,"%d OpenMP Target Capable devices are detected.\n", deviceCount);
   }

   int host_id = omp_get_initial_device();

   fprintf(stdout,"Host   ID             : %d\n", host_id);

   #pragma omp parallel
   {
   int host_id = omp_get_initial_device();

   fprintf(stdout,"Host   ID (in omp parallel) : %d\n", host_id);
   }


   int dev = 0;
   omp_set_default_device(dev);

   fprintf(stdout,"Device ID             : %d\n", dev);

   // I think there is no API to query hardware information of deveices in OpenMP. :( //

   return EXIT_SUCCESS;
}

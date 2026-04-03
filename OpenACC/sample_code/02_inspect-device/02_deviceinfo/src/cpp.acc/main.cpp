// Copyright 2025 Research Organization for Information Science and Technology
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <openacc.h>

#define PRINTAPI(x) printf("%s=%d\n", #x, x)

int main (int argc, char **argv)
{
   fprintf(stdout,"%s Starting ... \n", argv[0]);
   PRINTAPI(_OPENACC);

   acc_device_t dev_type = acc_get_device_type();

   int deviceCount = acc_get_num_devices(dev_type);

   if ( deviceCount == 0 ) {
      fprintf(stdout,"There is no available device.\n");
      return EXIT_SUCCESS;
   } else {
      fprintf(stdout,"%d OpenACC Capable devices are detected.\n", deviceCount);
   }

   int dev = 0;

   acc_set_device_num(dev, dev_type);

   uint64_t dev_mem = acc_get_property(dev, dev_type, acc_property_memory);

   fprintf(stdout,"Device %d: %s\n",
           dev,
           acc_get_property_string(dev, dev_type, acc_property_name) );
   fprintf(stdout,"  Driver:                                       %s\n",
           acc_get_property_string(dev, dev_type, acc_property_driver));
#if 0
   fprintf(stdout,"  Version:                                      %s\n",
           acc_get_property_string(dev, dev_type, acc_property_version));
   fprintf(stdout,"  Shared memory support:                        %s\n",
           acc_get_property_string(dev, dev_type, acc_property_shared_memory_support));
#endif
   fprintf(stdout,"  Total amount of device memory:                %.2f Mbytes (%llu bytes)\n",
           (double)dev_mem / (1048576.0), dev_mem);

   return EXIT_SUCCESS;
}

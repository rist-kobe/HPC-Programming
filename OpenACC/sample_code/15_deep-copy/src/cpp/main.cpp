// Copyright 2025 Research Organization for Information Science and Technology
// Reference
// * OpenACC Getting started guid, 2.5 and 2.9;
//   https://docs.nvidia.com/hpc-sdk/compilers/openacc-gs/index.html
#include <cstdio>
#include <cstdlib>
#include "myvec.h"

int main (int argc, char **argv)
{
   int nsize, nrep;
   double h_z, h_x, h_y;
  
   if ( argc != 2 ) {
      printf("[usage] run.x (arg1) \n");
      printf("  (arg1): Vector size (int) \n");
      return EXIT_SUCCESS;
   }

   nsize = atoi(argv[1]);

   if ( nsize <= 0 ) {
      nsize = 1 << 18;
   }

   nrep = 100;

   // for check
   h_x = 1.0 / static_cast<double>(nrep);
   h_y = 2.0 / static_cast<double>(nrep);
   h_z = 0.0; 
   for (int itr = 0; itr < nrep; ++itr) {
      h_z = h_x + h_y; 
   }

   //-------------------------------------------------------------------
   { // START
   //-------------------------------------------------------------------
   myvec<double> x(nsize);
   myvec<double> y(nsize);
   myvec<double> z(nsize);

   // set data in host
   for( int i =0; i< nsize; ++i ) {
      x[i] = 1.0 / static_cast<double>(nrep);
      y[i] = 2.0 / static_cast<double>(nrep);
      z[i] = 0.0;
   }

   // update data in device
   x.update_device();
   y.update_device();
   z.update_device();

   for (int itr = 0; itr < nrep; ++itr) {
      #pragma acc kernels loop independent present(z,x,y) \
      present(z.v[0:nsize],x.v[0:nsize],y.v[0:nsize])
      for (int i = 0; i < nsize; ++i ){
         z.v[i] = x.v[i] + y.v[i];
      }
   }

   // update data in host
   z.update_host();

   double chk = z.v[nsize/2] - h_z;
   fprintf(stdout,"%32s %18.6e\n", "Diff b/w device and host", chk);
   //-------------------------------------------------------------------
   } // END
   //-------------------------------------------------------------------

   return EXIT_SUCCESS;
}

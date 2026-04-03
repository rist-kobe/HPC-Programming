// Copyright 2025 Research Organization for Information Science and Technology
// Reference
// * OpenACC Getting started guid, 2.5 and 2.9;
//   https://docs.nvidia.com/hpc-sdk/compilers/openacc-gs/index.html
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include "myvec.h"

int dummy(const int itr)
{
  int icon = 0;
  return icon;
}

int main (int argc, char **argv)
{
   int nsize, nrep, icon;
   double h_z, h_x, h_y;
  
   if ( argc != 3 ) {
      printf("[usage] run.x (arg1) (arg2)\n");
      printf("  (arg1): Vector size (int) \n");
      printf("  (arg2): Number of iteration (int) \n");
      return EXIT_SUCCESS;
   }

   nsize = atoi(argv[1]);
   nrep  = atoi(argv[2]);

   if ( nsize <= 0 ) {
      nsize = 1 << 18;
   }

   if ( nrep <= 0 ) {
      nsize = 10000;
   }

   // for check
   h_x = 1.0 / static_cast<double>(nrep);
   h_y = 2.0 / static_cast<double>(nrep);
   h_z = 0.0; 
   for ( int itr = 0 ; itr < nrep; ++itr ){
      h_z = h_x + h_z*h_y; 
   }

   //-------------------------------------------------------------------
   { // Object-oriented implementation: Start
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

   // FMA in device: Overload
   {
   const auto elp0 = std::chrono::steady_clock::now(); 

   icon = 0;
   for ( int irep = 0; irep < nrep; ++irep) {
      z = x + z*y;
      icon += z.dummy(irep);
   }

   const std::chrono::duration<double> elp = std::chrono::steady_clock::now() - elp0; 
   
   z.update_host(); // update data in host

   double chk = z[nsize/2] - h_z;
   fprintf(stdout,"%15s,%13.4f,%18.6e\n", "fma_overload", static_cast<double>(elp.count()), chk);
   } 

   for( int i =0; i< nsize; ++i ) {
      z[i] = 0.0;
   }
   z.update_device(); // update data in device

   // FMA in device: Routine
   {
   const auto elp0 = std::chrono::steady_clock::now(); 

   icon = 0;
   for ( int irep = 0; irep < nrep; ++irep) {
      #pragma acc parallel 
      {
         z.run_fma(x,y);
      }
      icon += z.dummy(irep);
   }

   const std::chrono::duration<double> elp = std::chrono::steady_clock::now() - elp0; 
   
   z.update_host(); // update data in host

   double chk = z[nsize/2] - h_z;
   fprintf(stdout,"%15s,%13.4f,%18.6e\n", "fma_routine", static_cast<double>(elp.count()), chk);
   } 
   //-------------------------------------------------------------------
   } // Object-oriented implementation: End
   //-------------------------------------------------------------------

   //-------------------------------------------------------------------
   { // Low-level implementation: Start
   //-------------------------------------------------------------------
   // FMA in device: Explicit
   double *xx, *yy, *zz;
   xx = (double *)malloc( nsize*sizeof(double) );
   yy = (double *)malloc( nsize*sizeof(double) );
   zz = (double *)malloc( nsize*sizeof(double) );

   for( int i =0; i< nsize; ++i ) {
      xx[i] = 1.0 / static_cast<double>(nrep);
      yy[i] = 2.0 / static_cast<double>(nrep);
      zz[i] = 0.0;
   }

   #pragma acc enter data copyin(xx[0:nsize],yy[0:nsize],zz[0:nsize])

   const auto elp0 = std::chrono::steady_clock::now(); 

   icon = 0;
   for ( int irep = 0; irep < nrep; ++irep) {
      #pragma acc parallel loop gang vector
      for ( int i = 0; i < nsize; ++i ) {
         zz[i] = xx[i] + zz[i]*yy[i];
      }
      icon += dummy(irep);
   }

   const std::chrono::duration<double> elp = std::chrono::steady_clock::now() - elp0; 
   
   #pragma acc exit data copyout(zz[0:nsize])

   double chk = zz[nsize/2] - h_z;
   fprintf(stdout,"%15s,%13.4f,%18.6e\n", "fma_explicit", static_cast<double>(elp.count()), chk);
   //-------------------------------------------------------------------
   } // Low-level implementation: End
   //-------------------------------------------------------------------

   return EXIT_SUCCESS;
}

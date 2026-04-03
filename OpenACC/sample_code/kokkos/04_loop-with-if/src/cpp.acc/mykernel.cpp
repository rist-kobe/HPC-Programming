// Copyright 2023 Research Organization for Information Science and Technology
#include "mykernel.h"
#include <iostream>
#include <cmath>
#include "timer.h"
#include <openacc.h>

void MyKernel::device_memory(const int inum)
{
   d_il = (int *)acc_malloc( inum*sizeof(int) );
   d_x = (double *)acc_malloc( inum*sizeof(double) );
   d_y = (double *)acc_malloc( inum*sizeof(double) );
   d_z = (double *)acc_malloc( inum*sizeof(double) );
}

void MyKernel::memory_copy_to_device(const int inum)
{
   acc_memcpy_to_device(d_il, il, inum*sizeof(int) );
   acc_memcpy_to_device(d_x, x, inum*sizeof(double) );
   acc_memcpy_to_device(d_y, y, inum*sizeof(double) );
   acc_memcpy_to_device(d_z, z, inum*sizeof(double) );
}


void MyKernel::do_kernel (const int calctype, const int inum, const int nitr, 
  double rcut2, double *g, double &f, double &elp)
{
   double elp0;

   if ( calctype == 1 ) {
      #pragma acc data copy(g[0:inum]) deviceptr(d_il, d_x, d_y, d_z) 
      {
      elp0 = get_elp_time();
      for ( int it = 0; it < nitr; ++it ) {
         calc1(inum, rcut2, g, f); 
      }
      elp = get_elp_time() - elp0;
      } // acc data
   } else if (calctype ==2 ) {
      #pragma acc data copy(g[0:inum]) deviceptr(d_il, d_x, dy, dz) 
      {
      elp0 = get_elp_time();
      for ( int it = 0; it < nitr; ++it ) {
         calc2(inum, rcut2, g, f); 
      }
      elp = get_elp_time() - elp0;
      } // acc data
   }
}

void MyKernel::calc1 (int inum, double rcut2, double *g, double &f)
{
   setup();

   f = 0.0;
   #pragma acc kernels loop independent \
   present(g[0:inum])
   for (int i=0; i<inum; ++i) {
#if 0
      const int ii = il[i];
      const double dx = xt - x[ii];
      const double dy = yt - y[ii];
      const double dz = zt - z[ii];
      const double r2 = dx*dx + dy*dy + dz*dz;
      if ( r2 < rcut2 ) {
        const double u = exp(-0.5*r2/rcut2) / static_cast<double>(inum);
        g[ii] = x[ii] + y[ii] + z[ii] + u; 
      }
#else
      const int ii = d_il[i];
      const double dx = xt - d_x[ii];
      const double dy = yt - d_y[ii];
      const double dz = zt - d_z[ii];
      const double r2 = dx*dx + dy*dy + dz*dz;
      if ( r2 < rcut2 ) {
        const double u = exp(-0.5*r2/rcut2) / static_cast<double>(inum);
        g[ii] = d_x[ii] + d_y[ii] + d_z[ii] + u; 
      }
#endif
   }
}

void MyKernel::calc2 (int inum, double rcut2, double *g, double &f)
{
   setup();

   double f_ = 0.0;
   #pragma acc kernels loop independent reduction(+:f_) \
   present(g[0:inum]) copy(f_)
   for (int i=0; i<inum; ++i) {
#if 0
      const int ii = il[i];
      const double dx = xt - x[ii];
      const double dy = yt - y[ii];
      const double dz = zt - z[ii];
      const double r2 = dx*dx + dy*dy + dz*dz;
      if ( r2 < rcut2 ) {
        const double u = exp(-0.5*r2/rcut2) / static_cast<double>(inum);
        g[ii] = x[ii] + y[ii] + z[ii] + u; 
        f_ += u*dx + u*dy + u*dz; 
      }
#else
      const int ii = d_il[i];
      const double dx = xt - d_x[ii];
      const double dy = yt - d_y[ii];
      const double dz = zt - d_z[ii];
      const double r2 = dx*dx + dy*dy + dz*dz;
      if ( r2 < rcut2 ) {
        const double u = exp(-0.5*r2/rcut2) / static_cast<double>(inum);
        g[ii] = d_x[ii] + d_y[ii] + d_z[ii] + u; 
        f_ += u*dx*dx + u*dy*dy + u*dz*dz; 
      }
#endif
   }

   f = f_;
}

void MyKernel::calc2_alt (int inum, double rcut2, double * _restrict_cpp g, double &f)
{
   setup();

   f = 0.0;

   for (int i=0; i<inum; ++i) {
      const int ii = il[i];
      const double dx = xt - x[ii];
      const double dy = yt - y[ii];
      const double dz = zt - z[ii];
      const double r2 = dx*dx + dy*dy + dz*dz;
      if ( r2 < rcut2 ) {
        const double u = exp(-0.5*r2/rcut2);
        g[ii] = x[ii] + y[ii] + z[ii] + u; 
        f += u*dx + u*dy + u*dz; 
      }
   }
}

void MyKernel::calc2_mod (int inum, double rcut2, double * _restrict_cpp g, double &f)
{
   setup();

   int il_[MAXINUM];
   double dx_[MAXINUM];
   double dy_[MAXINUM];
   double dz_[MAXINUM];
   double r2_[MAXINUM];

   int ei = 0;

//#if defined(__CLANG_FUJITSU) 
//   #pragma clang loop vectorize(assume_safety)
//#elif __FUJITSU
//   #pragma loop norecurrence
//   #pragma loop novrec
//   #pragma loop noalias
//   #pragma loop simd 
//#endif
   for (int i=0; i<inum; ++i) {
      const int ii = il[i];
      const double dx = xt - x[ii];
      const double dy = yt - y[ii];
      const double dz = zt - z[ii];
      const double r2 = dx*dx + dy*dy + dz*dz;
      if ( r2 < rcut2 ) {
         il_[ei] = ii;
         dx_[ei] = dx;         
         dy_[ei] = dy;         
         dz_[ei] = dz;         
         r2_[ei] = r2;
         ei += 1;
      }
   }

   //std::cout << ei << std::endl;
   double f_ = 0.0;

   if ( ei < 32 ) {
#if defined(__CLANG_FUJITSU) 
      #pragma clang loop vectorize(disable)
#elif __FUJITSU
      #pragma loop nosimd
#endif
      for (int i=0; i < ei; ++i ){
         const int ii = il_[i];
         const double dx = dx_[i];
         const double dy = dy_[i];
         const double dz = dz_[i];
         const double r2 = r2_[i];
         const double u = exp(-0.5*r2/rcut2);
         g[ii] = x[ii] + y[ii] + z[ii] + u; 
         f_ += u*dx + u*dy + u*dz; 
      }
   } else {
//#if defined(__CLANG_FUJITSU) 
//      #pragma clang loop vectorize(assume_safety)
//#elif __FUJITSU
//      #pragma loop norecurrence
//      #pragma loop novrec
//      #pragma loop noalias
//      #pragma loop simd 
//#endif
      for (int i=0; i < ei; ++i ){
         const int ii = il_[i];
         const double dx = dx_[i];
         const double dy = dy_[i];
         const double dz = dz_[i];
         const double r2 = r2_[i];
         const double u = exp(-0.5*r2/rcut2);
         g[ii] = x[ii] + y[ii] + z[ii] + u; 
         f_ += u*dx + u*dy + u*dz; 
      }
   }

   f = f_;
}

void MyKernel::count_true (int inum, double rcut2, double &t_rate)
{
   setup();

   int ei = 0;
   for (int i=0; i<inum; ++i) {
      const int ii = il[i];
      const double dx = xt - x[ii];
      const double dy = yt - y[ii];
      const double dz = zt - z[ii];
      const double r2 = dx*dx + dy*dy + dz*dz;
      if ( r2 < rcut2 ) {
         ei += 1;
      }
   }

   t_rate = static_cast<double>(ei) / inum;
}

inline void MyKernel::setup()
{ 
  xt = 0.0;
  yt = 0.0;
  zt = 0.0;
}

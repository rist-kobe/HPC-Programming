// Copyright 2025 Research Organization for Information Science and Technology
#include "mykernel_kk.h"
#include <cmath>
#include "timer.h"
#include "type.h"

void MyKernel_kk::init(const int inum, const double rcut2, const int nstride, const double fac)
{
  nsize = inum;
  rc2 = rcut2;

  ViewVectorIType d_il_("MyKernell_kk.d_il", nsize);
  ViewVectorFType d_x_ ("MyKernell_kk.d_x" , nsize);
  ViewVectorFType d_y_ ("MyKernell_kk.d_y" , nsize);
  ViewVectorFType d_z_ ("MyKernell_kk.d_z" , nsize);
  ViewVectorFType d_g_ ("MyKernell_kk.d_z" , nsize);

  d_il = d_il_;
  d_x  = d_x_;
  d_y  = d_y_;
  d_z  = d_z_;
  d_g  = d_g_;

  h_il = Kokkos::create_mirror_view( d_il ); 
  h_x  = Kokkos::create_mirror_view( d_x  ); 
  h_y  = Kokkos::create_mirror_view( d_y  ); 
  h_z  = Kokkos::create_mirror_view( d_z  ); 
  h_g  = Kokkos::create_mirror_view( d_g  ); 

  for (int i = 0; i < nsize; ++i) {
     double tmp = 0.5*nsize - i;
     tmp *= fac;
     h_x( i ) = tmp;
     h_y( i ) = tmp + fac;
     h_z( i ) = tmp + 2.0*fac;
  }

  // Note: There is no overlap b/w the elements.
  for (int i = 0; i < nsize; ++i) {
     int ii = i + nstride;
     if ( ii >= nsize ) {
        ii = ii - nsize;
     }
     h_il( i ) = ii;
  }

  for ( int i = 0; i < nsize; ++i ) {
     h_g( i ) = 0.0;
  }

  // copy to device
  Kokkos::deep_copy( d_il, h_il );
  Kokkos::deep_copy( d_x, h_x );
  Kokkos::deep_copy( d_y, h_y );
  Kokkos::deep_copy( d_z, h_z ); 
  Kokkos::deep_copy( d_g, h_g ); // implicit barrier
}

void MyKernel_kk::do_kernel (const int calctype, const int nitr, double &f, double &elp)
{
   double elp0;

   if ( calctype == 1 ) {
      {
      elp0 = get_elp_time();
      for ( int it = 0; it < nitr; ++it ) {
         setup();
         f = 0.0;
	 Kokkos::parallel_for("MyKernel_kk::calc1", Kokkos::RangePolicy<TagA, ExecSpace>(0,nsize), *this); 
         Kokkos::fence();
      }
      elp = get_elp_time() - elp0;
      } 
   } else if (calctype ==2 ) {
      {
      elp0 = get_elp_time();
      for ( int it = 0; it < nitr; ++it ) {
         setup();
         f = 0.0;
	 Kokkos::parallel_reduce("MyKernel_kk::calc2", Kokkos::RangePolicy<TagB, ExecSpace>(0,nsize), *this, f); 
         Kokkos::fence();
      }
      elp = get_elp_time() - elp0;
      } 
   }
}

KOKKOS_INLINE_FUNCTION
void MyKernel_kk::operator() (TagA, const int i) const {
   const int ii = d_il( i );
   const double dx = xt - d_x( ii );
   const double dy = yt - d_y( ii );
   const double dz = zt - d_z( ii );
   const double r2 = dx*dx + dy*dy + dz*dz;
   if ( r2 < rc2 ) {
      const double u = exp(-0.5*r2/rc2) / static_cast<double>(nsize);
      d_g( ii ) = d_x( ii ) + d_y( ii ) + d_z( ii ) + u; 
   }
}

KOKKOS_INLINE_FUNCTION
void MyKernel_kk::operator() (TagB, const int i, double &f) const {
  const int ii = d_il( i );
  const double dx = xt - d_x( ii );
  const double dy = yt - d_y( ii );
  const double dz = zt - d_z( ii );
  const double r2 = dx*dx + dy*dy + dz*dz;
  if ( r2 < rc2 ) {
     const double u = exp(-0.5*r2/rc2) / static_cast<double>(nsize);
     d_g( ii ) = d_x( ii ) + d_y( ii ) + d_z( ii ) + u; 
     f += u*dx*dx + u*dy*dy + u*dz*dz; 
  }
}

void MyKernel_kk::count_true (double &t_rate)
{
   setup();

   int ei = 0;
   for (int i=0; i<nsize; ++i) {
      const int ii = h_il( i );
      const double dx = xt - h_x( ii );
      const double dy = yt - h_y( ii );
      const double dz = zt - h_z( ii );
      const double r2 = dx*dx + dy*dy + dz*dz;
      if ( r2 < rc2 ) {
         ei += 1;
      }
   }

   t_rate = static_cast<double>(ei) / nsize;
}

inline void MyKernel_kk::setup()
{ 
  xt = 0.0;
  yt = 0.0;
  zt = 0.0;
}

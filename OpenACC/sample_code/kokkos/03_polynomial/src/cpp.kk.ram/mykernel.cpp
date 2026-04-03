// Copyright 2025 Research Organization for Information Science and Technology
#include <cstdlib>
#include "mykernel.hpp"

MyKernel::MyKernel(const int ndim_)
{
  ndim = ndim_;
}

void MyKernel::create_data(unsigned int iseed, const FPtype drep)
{
  ViewVectorFType d_c0_("MyKernel::d_c0", ndim);
  d_c0 = d_c0_;
  ViewVectorFType d_c1_("MyKernel::d_c0", ndim);
  d_c1 = d_c1_;
  ViewVectorFType d_x_("MyKernel::d_c0", ndim);
  d_x = d_x_;

  h_c0 = Kokkos::create_mirror_view ( d_c0 );
  h_c1 = Kokkos::create_mirror_view ( d_c1 );
  h_x  = Kokkos::create_mirror_view ( d_x  );

  for ( int i=0; i<ndim; ++i ) h_x( i ) = 0.0 ;

  srand( iseed ) ;
  for( int i=0; i<ndim; ++i ) {
    FPtype tmp1 = static_cast<FPtype>(rand())/RAND_MAX ;
    FPtype tmp2 = static_cast<FPtype>(rand())/RAND_MAX ;
    h_c0( i )  = 2.0*(tmp1-0.5)*drep ;
    h_c1( i )  = 2.0*(tmp1-0.5)*drep/2.0 ;
  }

  Kokkos::deep_copy ( d_c0, h_c0 );
  Kokkos::deep_copy ( d_c1, h_c1 );
  Kokkos::deep_copy ( d_x, h_x ); // implicit barrier

#if 1
  drnd_c0 = d_c0;
  drnd_c1 = d_c1;
#endif

}

MyKernel::~MyKernel()
{
}

int MyKernel::dummy(int irep, FPtype drep)
{
   int icon = 0;
   return icon;
}

void MyKernel::poly(int deg)
{
   if ( deg == 1 ) {
      Kokkos::parallel_for("MyKernel::poly1", Kokkos::RangePolicy<TagP1, ExecSpace>(0, ndim), *this);
   } else if ( deg == 4 ) {
      Kokkos::parallel_for("MyKernel::poly4", Kokkos::RangePolicy<TagP4, ExecSpace>(0, ndim), *this);
   } else if ( deg == 8 ) {
      Kokkos::parallel_for("MyKernel::poly8", Kokkos::RangePolicy<TagP8, ExecSpace>(0, ndim), *this);
   } else if ( deg == 16 ) {
      Kokkos::parallel_for("MyKernel::poly16", Kokkos::RangePolicy<TagP16, ExecSpace>(0, ndim), *this);
   } else if ( deg == 32 ) {
      Kokkos::parallel_for("MyKernel::poly32", Kokkos::RangePolicy<TagP32, ExecSpace>(0, ndim), *this);
   }
}

KOKKOS_INLINE_FUNCTION
void MyKernel::operator() (TagP1, const int i) const 
{
#if 0
   d_x(i) = d_c0(i) + d_x(i)*d_c1(i);
#else
   d_x(i) = drnd_c0(i) + d_x(i)*drnd_c1(i);
#endif
}

KOKKOS_INLINE_FUNCTION
void MyKernel::operator() (TagP4, const int i) const 
{
#if 0
   d_x(i) = d_c0(i) 
           + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i) + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i)
           ))));
#else
   d_x(i) = drnd_c0(i) 
           + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i) + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i)
           ))));
#endif
}

KOKKOS_INLINE_FUNCTION
void MyKernel::operator() (TagP8, const int i) const 
{
#if 0
   d_x(i) = d_c0(i) 
           + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i) + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i)
           + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i) + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i)
           ))))))));
#else
   d_x(i) = drnd_c0(i) 
           + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i) + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i)
           + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i) + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i)
           ))))))));
#endif
}

KOKKOS_INLINE_FUNCTION
void MyKernel::operator() (TagP16, const int i) const
{
#if 0
   d_x(i) = d_c0(i) 
           + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i) + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i)
           + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i) + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i)
           + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i) + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i)
           + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i) + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i)
           ))))))))))))))));
#else
   d_x(i) = drnd_c0(i) 
           + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i) + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i)
           + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i) + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i)
           + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i) + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i)
           + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i) + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i)
           ))))))))))))))));
#endif
}

KOKKOS_INLINE_FUNCTION
void MyKernel::operator() (TagP32, const int i) const
{
#if 0
   d_x(i) = d_c0(i) 
           + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i) + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i)
           + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i) + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i)
           + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i) + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i)
           + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i) + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i)
           + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i) + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i)
           + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i) + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i)
           + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i) + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i)
           + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i) + d_x(i)*(d_c1(i) + d_x(i)*(d_c0(i)
           ))))))))))))))))))))))))))))))));
#else
   d_x(i) = drnd_c0(i) 
           + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i) + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i)
           + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i) + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i)
           + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i) + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i)
           + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i) + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i)
           + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i) + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i)
           + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i) + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i)
           + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i) + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i)
           + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i) + d_x(i)*(drnd_c1(i) + d_x(i)*(drnd_c0(i)
           ))))))))))))))))))))))))))))))));
#endif
}

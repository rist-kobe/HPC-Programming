// Copyright 2025 Research Organization for Information Science and Technology 
#include "mykernel_kk.hpp"
#include "type.hpp"

MyKernel_kk::MyKernel_kk (const int nvinp, const double ainp, const int ninp, const int minp, const int linp) 
{
   NV = nvinp;
   aa = ainp;

   NN = ninp;
   MM = minp;
   LL = linp;
}

MyKernel_kk::~MyKernel_kk()
{
}

void MyKernel_kk::init_daxpy()
{
   ViewVectorType d_y_("MyKernel_kk::d_y", NV);
   ViewVectorType d_x_("MyKernel_kk::d_x", NV);

   d_y = d_y_;
   d_x = d_x_;

   h_y = Kokkos::create_mirror_view ( d_y );
   h_x = Kokkos::create_mirror_view ( d_x );

   for (int i = 0; i < NV; ++i ) {
      h_y( i ) = 0.0; 
      h_x( i ) = static_cast<double>(i)/NV; 
   } 

   // Deep copy host view to device view
   Kokkos::deep_copy( d_y, h_y );
   Kokkos::deep_copy( d_x, h_x ); // implicit barrier
}

void MyKernel_kk::init_mv()
{
   ViewVectorType d_y_("MyKernel_kk::d_y", NN);
   ViewVectorType d_x_("MyKernel_kk::d_x", MM);
   ViewMatrixType d_A_("MyKernel_kk::d_A", NN, MM);

   d_y = d_y_;
   d_x = d_x_;
   d_A = d_A_;

   h_y = Kokkos::create_mirror_view ( d_y );
   h_x = Kokkos::create_mirror_view ( d_x );
   h_A = Kokkos::create_mirror_view ( d_A );

   for (int i = 0; i < NN; ++i ) {
      h_y( i ) = 0.0; 
   } 
   
   for (int j = 0; j < MM; ++j ) {
      h_x( j ) = static_cast<double>(j)/MM; 
   } 

   for (int i = 0; i < NN; ++i ) {
   for (int j = 0; j < MM; ++j ) {
      h_A( i, j ) = 1.0; 
   }}

   // Deep copy host view to device view
   Kokkos::deep_copy( d_y, h_y );
   Kokkos::deep_copy( d_x, h_x );
   Kokkos::deep_copy( d_A, h_A ); // implicit barrier
}

void MyKernel_kk::init_mm()
{
   ViewMatrixType d_A_("MyKernel_kk::d_A", NN, LL);
   ViewMatrixType d_B_("MyKernel_kk::d_B", LL, MM);
   ViewMatrixType d_C_("MyKernel_kk::d_C", NN, MM);

   d_A = d_A_;
   d_B = d_B_;
   d_C = d_C_;

   h_A = Kokkos::create_mirror_view ( d_A );
   h_B = Kokkos::create_mirror_view ( d_B );
   h_C = Kokkos::create_mirror_view ( d_C );

   for (int i = 0; i < NN; ++i ) {
   for (int k = 0; k < LL; ++k ) {
      h_A( i, k ) = 1.0; 
   }} 

   for (int k = 0; k < LL; ++k ) {
   for (int j = 0; j < MM; ++j ) {
      h_B( k, j ) = static_cast<double>(k)/LL;
   }}

   for (int i = 0; i < NN; ++i ) {
   for (int j = 0; j < MM; ++j ) {
      h_C( i, j ) = 0.0; 
   }} 

   // Deep copy host view to device view
   Kokkos::deep_copy( d_A, h_A );
   Kokkos::deep_copy( d_B, h_B );
   Kokkos::deep_copy( d_C, h_C ); // implicit barrier
}

void MyKernel_kk::daxpy_kk ()
{
  Kokkos::parallel_for("MyKernel_kk:daxpy_kk", 
     Kokkos::RangePolicy<TagAXPY,ExecSpace>(0, NV), *this);
}

void MyKernel_kk::mmp_simple_kk ()
{
  Kokkos::parallel_for("MyKernel_kk:mmp_simple_kk", 
     Kokkos::MDRangePolicy<TagMM,ExecSpace,Kokkos::Rank<2>>({0,0},{NN,MM}), *this);
}

int MyKernel_kk::dummy_kk (const int itr, const int ninp)
{
   int icon = 0;
   return icon;
}

KOKKOS_INLINE_FUNCTION
void MyKernel_kk::operator() (TagAXPY, const int i) const 
{
  d_y( i ) += aa*d_x( i );
}

KOKKOS_INLINE_FUNCTION
void MyKernel_kk::operator() (TagMM, const int i, const int j) const 
{
  // naive implementation
  double tmp = 0.0;
  for ( int k = 0; k < LL; ++k ) {
      tmp += d_A( i, k ) * d_B ( k, j );
  }
  d_C( i, j ) = tmp;
}

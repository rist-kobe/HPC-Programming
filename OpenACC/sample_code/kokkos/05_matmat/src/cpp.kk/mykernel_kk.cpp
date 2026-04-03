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
   d_y = d_y_;

   ViewVectorType d_x_("MyKernel_kk::d_x", NV);
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
   d_y = d_y_;

   ViewVectorType d_x_("MyKernel_kk::d_x", MM);
   d_x = d_x_;

   ViewMatrixType d_A_("MyKernel_kk::d_A", NN, MM);
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
   d_A = d_A_;

   ViewMatrixType d_B_("MyKernel_kk::d_B", LL, MM);
   d_B = d_B_;

   ViewMatrixType d_C_("MyKernel_kk::d_C", NN, MM);
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

void MyKernel_kk::init_mmnomd()
{
   ViewVectorType d_aa_("MyKernel_kk::d_aa", NN*LL);
   d_aa = d_aa_;
   
   ViewVectorType d_bb_("MyKernel_kk::d_bb", LL*MM);
   d_bb = d_bb_;

   ViewVectorType d_cc_("MyKernel_kk::d_cc", NN*MM);
   d_cc = d_cc_;

   h_aa = Kokkos::create_mirror_view ( d_aa );
   h_bb = Kokkos::create_mirror_view ( d_bb );
   h_cc = Kokkos::create_mirror_view ( d_cc );

   for (int i = 0; i < NN; ++i ) {
   for (int k = 0; k < LL; ++k ) {
      const int ik = k + i*LL;
      h_aa( ik ) = 1.0; 
   }} 

   for (int k = 0; k < LL; ++k ) {
   for (int j = 0; j < MM; ++j ) {
      const int kj = j + k*MM;
      h_bb( kj ) = static_cast<double>(k)/LL;
   }}

   for (int i = 0; i < NN; ++i ) {
   for (int j = 0; j < MM; ++j ) {
      const int ij = j + i*MM;	   
      h_cc( ij ) = 0.0; 
   }} 

   // Deep copy host view to device view
   Kokkos::deep_copy( d_aa, h_aa );
   Kokkos::deep_copy( d_bb, h_bb );
   Kokkos::deep_copy( d_cc, h_cc ); // implicit barrier
}

void MyKernel_kk::daxpy_kk ()
{
  Kokkos::parallel_for("MyKernel_kk:daxpy_kk", 
     Kokkos::RangePolicy<TagAXPY,ExecSpace>(0, NV), *this);
}

void MyKernel_kk::mvp_simple_kk ()
{
  Kokkos::parallel_for("MyKernel_kk:mvp_simple_kk", 
     Kokkos::RangePolicy<TagMV,ExecSpace>(0,NN), *this);
}

void MyKernel_kk::mmp_simple_kk ()
{
  Kokkos::parallel_for("MyKernel_kk:mmp_simple_kk", 
     Kokkos::MDRangePolicy<TagMM,ExecSpace,Kokkos::Rank<2>>({0,0},{NN,MM}), *this);
}

void MyKernel_kk::mmpnomd_simple_kk ()
{
  Kokkos::parallel_for("MyKernel_kk:mmpnomd_simple_kk", 
     Kokkos::RangePolicy<TagMMNOMD,ExecSpace>( 0, NN*MM ), *this);
}

void MyKernel_kk::mmp_hier_kk ()
{
  Kokkos::parallel_for("MyKernel_kk:mmp_hier_kk", 
     Kokkos::TeamPolicy<TagMMHIER,ExecSpace>(NN, Kokkos::AUTO, 32), *this);
}

void MyKernel_kk::mmp_hier2_kk ()
{
  const int vector_length = 8;
  //const int team_size = 32;
  Kokkos::parallel_for("MyKernel_kk:mmp_hier2_kk", 
     Kokkos::TeamPolicy<TagMMHIER2,ExecSpace>(NN, Kokkos::AUTO, vector_length), *this);
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
void MyKernel_kk::operator() (TagMV, const int i) const 
{
  double tmp = 0.0;
  for ( int j = 0; j < MM; ++j ) {
      tmp += d_A( i, j ) * d_x ( j );
  }
  d_y( i ) = tmp;
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

KOKKOS_INLINE_FUNCTION
void MyKernel_kk::operator() (TagMMNOMD, const int ij) const 
{
  // naive implementation
  const int i = ij / MM;
  const int j = ij - i*MM;
  double tmp = 0.0;
  for ( int k = 0; k < LL; ++k ) {
      const int ik = k + i*LL;
      const int kj = j + k*MM;
      tmp += d_aa( ik ) * d_bb( kj );
  }
  d_cc( ij ) = tmp;
}

KOKKOS_INLINE_FUNCTION
void MyKernel_kk::operator() (TagMMHIER, const typename Kokkos::TeamPolicy<ExecSpace>::member_type & team) const 
{
   // hierarchical parallelism
   // https://kokkos.org/kokkos-core-wiki/API/core/policies/TeamPolicy.html
   // https://kokkos.org/kokkos-core-wiki/API/core/policies/TeamVectorRange.html
   int i = team.league_rank();

   Kokkos::parallel_for(Kokkos::TeamVectorRange(team, 0, MM), [&] (const int & j ) {
     double tmp = 0.0;
     for ( int k = 0; k < LL; ++k ) {
         tmp += d_A( i, k ) * d_B ( k, j );
     }
     d_C( i, j ) = tmp;
   });
}

KOKKOS_INLINE_FUNCTION
void MyKernel_kk::operator() (TagMMHIER2, const typename Kokkos::TeamPolicy<ExecSpace>::member_type & team) const 
{
   // hierarchical parallelism
   // https://kokkos.org/kokkos-core-wiki/API/core/policies/TeamPolicy.html
   // https://kokkos.org/kokkos-core-wiki/API/core/policies/TeamThreadRange.html
   // https://kokkos.org/kokkos-core-wiki/API/core/policies/TeamVectorRange.html
   const int ns   = team.league_size();
   const int nblk = team.team_size(); 
   const int ista = team.league_rank()*nblk; 
   const int iend = ( (ista + nblk) < ns ) ? ista + nblk : ns; 

   Kokkos::parallel_for(Kokkos::TeamThreadRange(team, ista, iend), [&] (const int & i ) {
     Kokkos::parallel_for(Kokkos::ThreadVectorRange(team, 0, MM), [&] (const int & j ) {
        double tmp = 0.0;
        for ( int k = 0; k < LL; ++k ) {
           tmp += d_A( i, k ) * d_B ( k, j );
        }
        d_C( i, j ) = tmp;
     });
   });
}

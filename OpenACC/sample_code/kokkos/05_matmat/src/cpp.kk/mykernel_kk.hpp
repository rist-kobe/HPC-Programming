// Copyright 2025 Research Organization for Information Science and Technology 
#pragma once

#include "type.hpp"

class MyKernel_kk {
   public:
   ViewVectorType d_y;
   ViewVectorType d_x;
   ViewMatrixType d_A;
   ViewMatrixType d_B;
   ViewMatrixType d_C;
   ViewVectorType d_aa;
   ViewVectorType d_bb;
   ViewVectorType d_cc;
 
   HostViewVectorType h_y;
   HostViewVectorType h_x;
   HostViewMatrixType h_A;
   HostViewMatrixType h_B;
   HostViewMatrixType h_C;
   HostViewVectorType h_aa;
   HostViewVectorType h_bb;
   HostViewVectorType h_cc;

   struct TagAXPY {};
   struct TagMV {};
   struct TagMM {};
   struct TagMMNOMD {};
   struct TagMMHIER {};
   struct TagMMHIER2 {};

   MyKernel_kk() {};
   MyKernel_kk(const int, const double, const int, const int, const int);
   ~MyKernel_kk();

   void init_daxpy ();
   void init_mv ();
   void init_mm ();
   void init_mmnomd ();

   void daxpy_kk ();
   KOKKOS_INLINE_FUNCTION
   void operator() (TagAXPY, const int) const;

   void mvp_simple_kk ();
   KOKKOS_INLINE_FUNCTION
   void operator() (TagMV, const int) const;

   void mmp_simple_kk ();
   KOKKOS_INLINE_FUNCTION
   void operator() (TagMM, const int, const int) const;

   void mmpnomd_simple_kk ();
   KOKKOS_INLINE_FUNCTION
   void operator() (TagMMNOMD, const int) const;

   void mmp_hier_kk ();
   KOKKOS_INLINE_FUNCTION
   void operator() (TagMMHIER, const typename Kokkos::TeamPolicy<ExecSpace>::member_type &) const ;

   void mmp_hier2_kk ();
   KOKKOS_INLINE_FUNCTION
   void operator() (TagMMHIER2, const typename Kokkos::TeamPolicy<ExecSpace>::member_type &) const ;

   int dummy_kk (const int, const int);

   private:
   int NV;
   double aa;
   int NN;
   int MM;
   int LL;
};

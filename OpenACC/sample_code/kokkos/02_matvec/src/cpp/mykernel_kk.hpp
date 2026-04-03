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
 
   HostViewVectorType h_y;
   HostViewVectorType h_x;
   HostViewMatrixType h_A;
   HostViewMatrixType h_B;
   HostViewMatrixType h_C;

   struct TagAXPY {};
   struct TagMV {};
   struct TagMM {};

   MyKernel_kk() {};
   MyKernel_kk(const int, const double, const int, const int, const int);
   ~MyKernel_kk();

   void init_daxpy ();
   void init_mv ();
   void init_mm ();

   void daxpy_kk ();
   KOKKOS_INLINE_FUNCTION
   void operator() (TagAXPY, const int) const;

   void mvp_simple_kk ();
   KOKKOS_INLINE_FUNCTION
   void operator() (TagMV, const int) const;

   void mmp_simple_kk ();
   KOKKOS_INLINE_FUNCTION
   void operator() (TagMM, const int, const int) const;

   int dummy_kk (const int, const int);

   private:
   int NV;
   double aa;
   int NN;
   int MM;
   int LL;
};

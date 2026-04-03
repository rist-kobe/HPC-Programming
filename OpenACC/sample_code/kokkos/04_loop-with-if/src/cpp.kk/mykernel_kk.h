// Copyright 2025 Research Organization for Information Science and Technology
#ifndef MYKERNEL_H
#define MYKERNEL_H

#include "type.h"

//#define MAXINUM 2000
#define MAXINUM 50000

class MyKernel_kk {
 public:
  ViewVectorIType d_il;
  ViewVectorFType d_x;
  ViewVectorFType d_y;
  ViewVectorFType d_z;
  ViewVectorFType d_g;

  HostViewVectorIType h_il;
  HostViewVectorFType h_x;
  HostViewVectorFType h_y;
  HostViewVectorFType h_z;
  HostViewVectorFType h_g;

  struct TagA {};
  struct TagB {};

  void init(const int, const double, const int, const double);

  void do_kernel (const int, const int, double &, double &);
  KOKKOS_INLINE_FUNCTION
  void operator() (TagA, const int ) const;
  KOKKOS_INLINE_FUNCTION
  void operator() (TagB, const int, double &) const;

  void count_true (double &);

 private:
  int nsize; 
  double xt;
  double yt;
  double zt;
  double rc2;
  inline void setup();
};

#endif //MYKERNEL_H

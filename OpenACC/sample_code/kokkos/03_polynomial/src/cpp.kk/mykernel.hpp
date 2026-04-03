// Copyright 2024 Research Organization for Information Science and Technology
#ifndef __MYKERNEL_H__
#define __MYKERNEL_H__

#include "types.hpp"

class MyKernel
{
  public:
  ViewVectorFType d_c0;
  ViewVectorFType d_c1;
  ViewVectorFType d_x;

  HostViewVectorFType h_c0;
  HostViewVectorFType h_c1;
  HostViewVectorFType h_x;

  struct TagP1 {};
  struct TagP4 {};
  struct TagP8 {};
  struct TagP16 {};
  struct TagP32 {};

  MyKernel() {};
  MyKernel(const int);
  ~MyKernel();

  void create_data(unsigned int, const FPtype);
  int dummy (int, FPtype);

  void poly (int deg);
  KOKKOS_INLINE_FUNCTION
  void operator() (TagP1, const int) const;
  KOKKOS_INLINE_FUNCTION
  void operator() (TagP4, const int) const;
  KOKKOS_INLINE_FUNCTION
  void operator() (TagP8, const int) const;
  KOKKOS_INLINE_FUNCTION
  void operator() (TagP16, const int) const;
  KOKKOS_INLINE_FUNCTION
  void operator() (TagP32, const int) const;

  private:
  int ndim;
};
#endif // __MYKERNEL_H__

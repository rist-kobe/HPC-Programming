// Copyright 2025 Research Organization for Information Science and Technology
#ifndef TYPE_H
#define TYPE_H

#include <Kokkos_Core.hpp>

#if ! defined(KOKKOS_ENABLE_CUDA)
#error "Default setting is to use CUDA wit Kokkos_ENABLE_CUDA=ON"
#endif

typedef Kokkos::Cuda        ExecSpace;
typedef Kokkos::CudaSpace   MemSpace;
typedef Kokkos::LayoutRight Layout; // for C/C++
//#if defined(USE_CUDAUVM)
//typedef  Kokkos::CudaUVMSpace  MemSpace;  
//#endif
typedef Kokkos::View<int *, Layout, MemSpace>     ViewVectorIType;
typedef Kokkos::View<double *, Layout, MemSpace>  ViewVectorFType;
typedef ViewVectorIType::HostMirror HostViewVectorIType;
typedef ViewVectorFType::HostMirror HostViewVectorFType;

//typedef Kokkos::RangePolicy<ExecSpace> range_policy;

#endif // TYPE_H

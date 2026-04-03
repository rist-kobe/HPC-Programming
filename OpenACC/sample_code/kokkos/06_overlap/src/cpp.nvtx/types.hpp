// Copyright 2025 Research Organization for Information Science and Technology 
#pragma once

#include <Kokkos_Core.hpp>

#if ! defined(KOKKOS_ENABLE_CUDA)
#error "Default setting is to use CUDA wit Kokkos_ENABLE_CUDA=ON"
#endif

#include "omp.h"

#define CHECK(call)                                                                 \
{                                                                                   \
   cudaError_t error = call;                                                        \
   if ( error != cudaSuccess )                                                      \
   {                                                                                \
      fprintf(stderr,"Error: %s:%d ", __FILE__, __LINE__ );                         \
      fprintf(stderr,"returned %d -> %s\n", (int)error, cudaGetErrorString(error)); \
      exit(EXIT_FAILURE);                                                           \
   }                                                                                \
}

typedef Kokkos::OpenMP             HostExecSpace;
typedef Kokkos::HostSpace          HostMemSpace;

typedef Kokkos::Cuda               DeviceExecSpace;
#if defined(USE_CUDAUVM)
typedef Kokkos::CudaUVMSpace       DeviceMemSpace;
#else
typedef Kokkos::CudaSpace          DeviceMemSpace;
#endif // defined(USE_CUDAUVM)

//#if ! defined(USE_LAYOUTLEFT)
// for C/C++
//typedef Kokkos::LayoutRight  Layout;
//#else
// for Fortran
//typedef Kokkos::LayoutLeft   Layout;
//#endif

#if defined(USE_FP_DP)
typedef double FPtype;
#elif defined(USE_FP_SP)
typedef float FPtype;
#else
typedef float FPtype;
#endif

typedef Kokkos::View<int * , Kokkos::LayoutRight, DeviceMemSpace>     ViewVectorIType;
typedef Kokkos::View<FPtype * , Kokkos::LayoutRight, DeviceMemSpace>  ViewVectorFType;
typedef Kokkos::View<int **, Kokkos::LayoutRight, DeviceMemSpace>     ViewMatrixIType;
typedef Kokkos::View<FPtype **, Kokkos::LayoutRight, DeviceMemSpace>  ViewMatrixFType;

//typedef Kokkos::View<FPtype *,Kokkos::LayoutRight,Kokkos::MemoryTraits<Kokkos::Atomic>, MemSpace>        ViewVectorAtomicFType;
typedef Kokkos::View<const int *,Kokkos::LayoutRight,Kokkos::MemoryTraits<Kokkos::RandomAccess>, DeviceMemSpace>     ViewVectorRNDIType;
typedef Kokkos::View<const FPtype *,Kokkos::LayoutRight,Kokkos::MemoryTraits<Kokkos::RandomAccess>, DeviceMemSpace>  ViewVectorRNDFType;
typedef Kokkos::View<const int **,Kokkos::LayoutRight,Kokkos::MemoryTraits<Kokkos::RandomAccess>, DeviceMemSpace>    ViewMatrixRNDIType;
typedef Kokkos::View<const FPtype **,Kokkos::LayoutRight,Kokkos::MemoryTraits<Kokkos::RandomAccess>, DeviceMemSpace> ViewMatrixRNDFType;

typedef ViewVectorIType::HostMirror HostViewVectorIType;
typedef ViewVectorFType::HostMirror HostViewVectorFType;
typedef ViewMatrixIType::HostMirror HostViewMatrixIType;
typedef ViewMatrixFType::HostMirror HostViewMatrixFType;

//typedef Kokkos::RangePolicy<ExecSpace>                     range_policy; 
//typedef Kokkos::MDRangePolicy<ExecSpace, Kokkos::Rank<2>>  mdrange_policy;  

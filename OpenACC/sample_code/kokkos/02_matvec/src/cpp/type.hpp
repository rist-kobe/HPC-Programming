// Copyright 2025 Research Organization for Information Science and Technology 
#pragma once

#include <Kokkos_Core.hpp>

#if ! defined(KOKKOS_ENABLE_CUDA)
#error "Default setting is to use CUDA wit Kokkos_ENABLE_CUDA=ON"
#endif

#if defined(USE_OPENMP) || defined(USE_OPENMPTARGET)
#include "omp.h"
#else // default: CUDA
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
#endif

#if defined(USE_SERIAL)
typedef Kokkos::Serial             ExecSpace;
typedef Kokkos::HostSpace          MemSpace;
#elif defined(USE_OPENMP)
typedef Kokkos::OpenMP             ExecSpace;
typedef Kokkos::HostSpace          MemSpace;
#elif defined(USE_OPENMPTARGET)
typedef Kokkos::OpenMPTarget       ExecSpace;
typedef Kokkos::OpenMPTargetSpace  MemSpace;
#else  // default: CUDA without UVM
typedef Kokkos::Cuda               ExecSpace;
#if defined(USE_CUDAUVM)
typedef Kokkos::CudaUVMSpace       MemSpace;
#else
typedef Kokkos::CudaSpace          MemSpace;
#endif // defined(USE_CUDAUVM)
#endif

//#if ! defined(USE_LAYOUTLEFT)
// for C/C++
//typedef Kokkos::LayoutRight  Layout;
//#else
// for Fortran
//typedef Kokkos::LayoutLeft   Layout;
//#endif

typedef Kokkos::View<double * , MemSpace>  ViewVectorType;
typedef Kokkos::View<double **, Kokkos::LayoutRight, MemSpace>  ViewMatrixType;
typedef ViewVectorType::HostMirror HostViewVectorType;
typedef ViewMatrixType::HostMirror HostViewMatrixType;

//typedef Kokkos::RangePolicy<ExecSpace>                     range_policy; 
//typedef Kokkos::MDRangePolicy<ExecSpace, Kokkos::Rank<2>>  mdrange_policy;  

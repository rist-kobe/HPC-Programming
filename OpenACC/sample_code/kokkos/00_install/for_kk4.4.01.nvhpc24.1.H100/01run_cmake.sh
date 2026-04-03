#!/bin/bash
# Load sutiable module settings for CMake and NVIDIA HPC SDK if necessary.
# Example:
# module purge
# module load cmake/3.28.3 
# module load nvhpc/24.1

PREFIX= # Set your install location
CMAKE_EXE=cmake
CMAKE_PRESET=nvhpc.cmake

export TMPDIR= # Set a temporary directory (any place)

if [ ! -d ${TMPDIR} ] ; then
   mkdir -p ${TMPDIR}
fi

#echo $NVHPC_ROOT
#echo $CUDA_PATH

${CMAKE_EXE} \
-C ${CMAKE_PRESET} \
-DCMAKE_INSTALL_PREFIX=${PREFIX} \
-DBUILD_SHARED_LIBS=on \
-DKokkos_ARCH_ZEN3=on \
-DKokkos_ARCH_HOPPER90=on \
-DKokkos_ENABLE_SERIAL=on \
-DKokkos_ENABLE_OPENMP=on \
-DKokkos_ENABLE_CUDA=on \
-DKokkos_ENABLE_BENCHMARKS=off \
-DKokkos_ENABLE_EXAMPLES=off \
-DKokkos_ENABLE_TESTS=off \
-DKokkos_ENABLE_TUNING=off \
-DKokkos_ENABLE_AGGRESSIVE_VECTORIZATION=off \
-DKokkos_ENABLE_DEBUG=off \
-DKokkos_ENABLE_CUDA_CONSTEXPR=off \
-DKokkos_ENABLE_CUDA_LAMBDA=on \
-DKokkos_ENABLE_COMPILER_WARNINGS=off \
-DKokkos_CUDA_DIR=${NVHPC_ROOT}/cuda/12.3 \
..


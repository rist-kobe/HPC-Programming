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

make -j 8
make install

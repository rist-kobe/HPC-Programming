#!/bin/bash

export KOKKOS_ROOT=
export KOKKOS_LIBDIR=
export LD_LIBRARY_PATH=${KOKKOS_LIBDIR}:${LD_LIBRARY_PATH}

export OMP_THREAD_NUM=1
export OMP_PLACES=cores
export OMP_PROC_BIND=close

echo -n "START: " 
date
ulimit -s unlimited

EXE=../../cpp.hierarchical/stream

# Automatically set by KOKKOS
ID=0
VL=-1
${EXE} -1 ${VL} &> out.${ID}.log

# Explicitly control hierarchical parallelism
ID=1
VL=32
${EXE} -1 ${VL} &> out.${ID}.log

ID=2
VL=64
${EXE} -1 ${VL} &> out.${ID}.log

ID=3
VL=128
${EXE} -1 ${VL} &> out.${ID}.log

ID=4
VL=256
${EXE} -1 ${VL} &> out.${ID}.log

echo -n "END: " 
date

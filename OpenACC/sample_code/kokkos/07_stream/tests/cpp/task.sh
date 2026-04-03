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

EXE=../../cpp/stream

ID=0
${EXE} -1 -1  &> out.${ID}.log

#!/bin/bash

echo -n "START: "
date
ulimit -s unlimited

COMPILER=nvfortran
${COMPILER} --version
#lscpu
nvidia-smi -L
nvidia-smi topo -m

#export NVCOMPILER_ACC_TIME=1
#export NVCOMPILER_ACC_NOTIFY=3

BINDIR=../../c/fortran/
EXE=${BINDIR}/run.x

${EXE} &> out.log 

echo -n "END: " 
date

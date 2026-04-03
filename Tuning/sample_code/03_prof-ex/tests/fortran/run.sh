#!/bin/bash

echo -n "START: " 
date
ulimit -s unlimited

# Set load module (absolute path could be preferable)
EXE=$(echo "../../src/fortran/diffuse.x")

COMPILER=gfortran
${COMPILER} --version
uname -a
lscpu

# Run
${EXE} 0 &> diffuse.out

# Get profiler data 
sleep 10s
gprof -p ${EXE} > flat.out

gprof -q ${EXE} > call.out

echo -n "END: " 
date

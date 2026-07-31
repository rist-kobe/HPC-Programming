#!/bin/bash

echo -n "START: " 
date
ulimit -s unlimited

# Set load module (absolute path could be preferable)
EXE=$(echo "../../src/fortran/run.x")

COMPILER=gfortran

${COMPILER} --version
uname -a
lscpu

# Run
$EXE > outfile

# Get profiler data 
# If setting -pg in the compiler option, uncomment the following two lines
#sleep 10s
#gprof $EXE > prof.out

echo -n "END: " 
date

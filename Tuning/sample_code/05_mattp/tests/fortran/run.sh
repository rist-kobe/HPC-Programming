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

# Set matrix row size (< 8192)
NROW=1000

# Set block size ( < NROW )
NBK=10

# Run
$EXE $NROW $NBK > outlist

echo -n "END: " 
date

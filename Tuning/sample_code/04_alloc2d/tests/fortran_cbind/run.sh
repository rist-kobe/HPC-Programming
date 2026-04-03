#! /bin/bash

echo -n "START: " 
date
ulimit -s unlimited

# Set load module (absolute path could be preferable)
EXE=$(echo "../../src/fortran_cbind/run.x")

COMPILER=gfortran
${COMPILER} --version
uname -a
lscpu

# Run
$EXE > outlist

echo -n "END: " 
date

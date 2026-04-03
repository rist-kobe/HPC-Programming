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
${EXE} &> outfile

echo -n "END: " 
date

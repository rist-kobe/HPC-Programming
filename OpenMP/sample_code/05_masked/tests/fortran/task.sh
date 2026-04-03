#! /bin/bash

export OMP_NUM_THREADS=4
export OMP_DISPLAY_ENV=TRUE

echo -n "START: " 
date
ulimit -s unlimited

COMPILER=gfortran
${COMPILER} --version
uname -a
lscpu

BINDIR=../../src/fortran
EXE=${BINDIR}/run.x

${EXE} 

echo -n "END: " 
date
### End of script

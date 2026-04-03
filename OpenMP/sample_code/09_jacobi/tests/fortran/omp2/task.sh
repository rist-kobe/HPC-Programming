#! /bin/bash 
### Threads in OpenMP 
export OMP_NUM_THREADS=6
export OMP_PLACES=cores
export OMP_PROC_BIND=close
export OMP_DISPLAY_ENV=TRUE
export OMP_STACKSIZE=1G


echo -n "START: " 
date

ulimit -s unlimited

COMPILER=gfortran
${COMPILER} --version
uname -a
lscpu

### Load module
BINDIR=../../../src/fortran/omp2
EXE=${BINDIR}/run.x

### Run
time ${EXE} > logfile

echo -n "END: " 
date

#! /bin/bash 
### Threads in OpenMP 
export OMP_NUM_THREADS=1
export OMP_WAIT_POLICY=ACTIVE
export OMP_PLACES=cores
export OMP_PROC_BIND=close
export OMP_DISPLAY_ENV=TRUE
export OMP_STACKSIZE=1G

echo -n "START: " 
date
ulimit -s unlimited

COMPILER=gcc
${COMPILER} --version
uname -a
lscpu

### Load module
BINDIR=../../../src/c/serial_delcopy
EXE=${BINDIR}/run.x

time ${EXE} > logfile

echo -n "END: " 
date

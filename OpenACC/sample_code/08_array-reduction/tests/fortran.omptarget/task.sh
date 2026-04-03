#! /bin/bash 

echo -n "START: " 
date
ulimit -s unlimited

uname -a
#lscpu
nvidia-smi -L

### Load module
BINDIR=../../src/fortran.omptarget
EXE=${BINDIR}/run.x

### Run
#export OMP_DYNAMIC=FALSE
export OMP_WAIT_POLICY=ACTIVE
#export OMP_STACKSIZE=1G
#export OMP_SCHEDULE="STATIC"
#export OMP_DISPLAY_ENV=TRUE
export OMP_PLACES=cores
export OMP_PROC_BIND=close
export OMP_NUM_THREADS=1
export OMP_TARGET_OFFLOAD=mandatory
${EXE} 1> out.log 2> err.log

echo -n "END: " 
date

### End of script

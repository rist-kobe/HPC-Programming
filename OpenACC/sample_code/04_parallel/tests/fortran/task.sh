#! /bin/bash 
echo -n "START: " 
date
ulimit -s unlimited

COMPILER=nvfortran
${COMPILER} --version
uname -a
lscpu
nvidia-smi -L

### Load module
BINDIR=../../src/fortran
EXE=${BINDIR}/run.x

### for Nvidia HPC SDK: Do not set those when using nsys 
#export NVCOMPILER_ACC_TIME=1
#export NVCOMPILER_ACC_NOTIFY=3 #4 #8 #16

### Run
time ${EXE} &> out.log

echo -n "END: " 
date
### End of script

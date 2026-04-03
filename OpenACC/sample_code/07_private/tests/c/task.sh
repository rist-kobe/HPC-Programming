#! /bin/bash 
echo -n "START: " 
date
ulimit -s unlimited

COMPILER=nvc
${COMPILER} --version
uname -a
lscpu
nvidia-smi -L

### Load module
BINDIR=../../src/c
EXE=${BINDIR}/run.x

### for Nvidia HPC SDK: Do not set those when using nsys 
export NVCOMPILER_ACC_TIME=1
#export NVCOMPILER_ACC_NOTIFY=3 #4 #8 #16

### Run
time -P ${EXE} 1>out.log 2>err.log

echo -n "END: " 
date
### End of script

#! /bin/bash 
### Threads in OpenMP 
export OMP_NUM_THREADS=36
export OMP_WAIT_POLICY=ACTIVE
export OMP_PLACES=cores
export OMP_PROC_BIND=close
#export OMP_DISPLAY_ENV=TRUE
export OMP_STACKSIZE=1G

### For Nvidia HPC SDK: Do not set those when using nsys 
#export NVCOMPILER_ACC_TIME=1
#export NVCOMPILER_ACC_NOTIFY=3 #4 #8 #16

echo -n "START: " 
date
ulimit -s unlimited

COMPILER=nvc
${COMPILER} --version
uname -a
lscpu
nvidia-smi -L

### Load module
BINDIR=../../../src/fortran/acc1
EXE=${BINDIR}/run.x

PERF=$(echo "nsys profile")
#PERFOPT=$(echo "--stats=true --trace=cuda,openacc")
PERFOPT=$(echo "--stats=true --trace=cuda,openacc,nvtx")
#PERFOPT=$(echo "--stats=true --trace=cuda,openacc,nvtx --cuda-um-cpu-page-faults=true --cuda-um-gpu-page-faults=true")

${PERF} ${PERFOPT} \
${EXE} 1> out.log 2> err.log

echo -n "END: " 
date

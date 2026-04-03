#! /bin/bash 
### For Nvidia HPC SDK: Do not set those when using nsys 
#export NVCOMPILER_ACC_TIME=1
#export NVCOMPILER_ACC_NOTIFY=3 #4 #8 #16

echo -n "START: " 
date
ulimit -s unlimited

COMPILER=nvfortran
${COMPILER} --version
uname -a
lscpu
nvidia-smi -L

### Load module
BINDIR=../../../src/fortran/noasync
EXE=${BINDIR}/run.x

PERF=$(echo "nsys profile")
#PERFOPT=$(echo "--stats=true --trace=cuda,openacc")
PERFOPT=$(echo "--stats=true --trace=cuda,openacc,nvtx")
#PERFOPT=$(echo "--stats=true --trace=cuda,openacc,nvtx --cuda-um-cpu-page-faults=true --cuda-um-gpu-page-faults=true")

${PERF} ${PERFOPT} \
${EXE} 1> out.log 2> err.log

echo -n "END: " 
date

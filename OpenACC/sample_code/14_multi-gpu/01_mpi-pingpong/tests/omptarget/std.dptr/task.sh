#!/bin/bash

# This jobscript is suitable for OpenMPI with ucx infitiband (ucx_ib)
# We recommend checking the output of "ucx_info -cf" before execution.

export OMP_NUM_THREADS=1
export OMP_TARGET_OFFLOAD=mandatory

BINDIR=../../../src/omptarget/std.dptr
EXE=${BINDIR}/run.x

ID=0
PPN=1
NCPUS=2 
export UCX_IB_GPU_DIRECT_RDMA=try 
time mpiexec --npernode ${PPN} --np ${NCPUS} --bind-to core -x LD_LIBRARY_PATH \
bash go.sh ${EXE} ${COUNT}

ID=1
PPN=1
NCPUS=2 
export UCX_IB_GPU_DIRECT_RDMA=yes 
time mpiexec --npernode ${PPN} --np ${NCPUS} --bind-to core -x LD_LIBRARY_PATH \
bash go.sh ${EXE} ${COUNT}

ID=2
PPN=1
NCPUS=2 
export UCX_IB_GPU_DIRECT_RDMA=no 
time mpiexec --npernode ${PPN} --np ${NCPUS} --bind-to core -x LD_LIBRARY_PATH \
bash go.sh ${EXE} ${COUNT}

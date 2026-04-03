#! /bin/bash 
# This script is suitable for OpenMPI

ND=1    # Number of nodes
PPN=2   # Processes per node. It must be equal to num of GPUs per node.
NP=$((ND*PPN))
NPX=${NP}
NPY=1 # NPX x NPY = NP

echo -n "START: " 
date
ulimit -s unlimited

### Load module
BINDIR=../../../src/c/acc1
EXE=${BINDIR}/run.x

PERF=time #$(echo "nsys profile")
PERFOPT=  #$(echo "--stats=true --trace=cuda,openacc,nvtx,ucx")
MPIEXE=mpiexec
MPIOPT=$(echo "--npernode ${PPN} --np ${NP} --bind-to core -x LD_LIBRARY_PATH") # for pure MPI + GPU
MPIPARM= #$(echo "-mca coll_hcoll_enable 0") # if use of HCOLL is off

# Note: 
# The sample code is written so that only rank 0 has file I/O, 
# but careless use of redirection in MPI code may be dangerous. We do not 
# recommend use of the following script in your production run.
${PERF} ${PERFOPT} \
${MPIEXE} ${MPIOPT} ${MPIPARM} \
${EXE} 8192 8192 ${NPX} ${NPY} 1> out.log 2> err.log 

# A better way, but it may be still problematic.
# exec_program=${EXE} npx_var=${NPX} npy_var=${NPY} \
# ${PERF} ${PERFOPT} \
# ${MPIEXE} ${MPIOPT} ${MPIPARM} bash -c '\
# OUTPUT=out.log.${OMPI_COMM_WORLD_RANK} && \
# ERRLOG=err.log.${OMPI_COMM_WORLD_RANK} && \
# ${exe_program} 8192 8192 ${npx_var} ${npy_var} 1> ${OUTPUT} 2> ${ERRLOG} '

echo -n "END: " 
date

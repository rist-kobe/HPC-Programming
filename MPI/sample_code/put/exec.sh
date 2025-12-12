#!/bin/bash
#SBATCH --nodes=2              ## SLURM_JOB_NUM_NODES
#SBATCH --ntasks-per-node=2    ## SLURM_TASKS_PER_NODE
#SBATCH --cpus-per-task=4      ## SLURM_CPUS_PER_TASK
#SBATCH --output=%x_%j.stdout  ## %x : SLURM_JOB_NAME
#SBATCH --error=%x_%j.stderr   ## %j : SLURM_JOB_ID
set -x
#
LOAD_MODULE_PATH=.
LOAD_MODULE_NAME=${1}
NUM_PROCS=4
MAX_PROCS_PER_NODE=2
MACHINE_FILE_NAME=${SLURM_JOB_NAME}_${SLURM_JOB_ID}.machinefile
#
mpicc -o ${1} ${1}.c
#
echo ${SLURM_JOB_NODELIST} | \
    awk -f mk_machinefile.awk -v mppn=${MAX_PROCS_PER_NODE} > ${MACHINE_FILE_NAME}
#
mpiexec -n ${NUM_PROCS} --machinefile ${MACHINE_FILE_NAME} \
    bash numa_bind_exec.sh ${MAX_PROCS_PER_NODE} \
    ${LOAD_MODULE_PATH} ${LOAD_MODULE_NAME}
#

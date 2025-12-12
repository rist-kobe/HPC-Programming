#!/bin/bash
#SBATCH --nodes=1              ## SLURM_JOB_NUM_NODES
#SBATCH --ntasks-per-node=1    ## SLURM_TASKS_PER_NODE
#SBATCH --cpus-per-task=4      ## SLURM_CPUS_PER_TASK
#SBATCH --output=%x_%j.stdout  ## %x : SLURM_JOB_NAME
#SBATCH --error=%x_%j.stderr   ## %j : SLURM_JOB_ID
set -x
#
LOAD_MODULE_PATH=.
LOAD_MODULE_NAME=${1}
NUM_PROCS=1
#
mpicc -o ${1} ${1}.c
#
mpiexec -n ${NUM_PROCS} \
    ${LOAD_MODULE_PATH}/${LOAD_MODULE_NAME}
#

#!/bin/bash

#module purge
#module load 

ulimit -s unlimited
nvidia-smi -L

KOKKOS_ROOT= # Specify
EXE=../run.x 

export LD_LIBRARY_PATH=${KOKKOS_ROOT}/lib64:${LD_LIBRARY_PATH}

export OMP_NUM_THREADS=${NT}
export OMP_WAIT_POLICY=ACTIVE
export OMP_STACKSIZE=256M
export OMP_PLACES=cores
export OMP_PROC_BIND=close

# Execution
PERF=time
PERFOPT= 

date > STAMP
# Warm-up
ICNT=4 # 16
CMDOPT=$(echo "${ICNT} 10")
${PERF} ${PERFOPT}                     \
${EXE} ${CMDOPT} &> /dev/null

ICNT=6 # 64
CMDOPT=$(echo "${ICNT} 100000")
${PERF} ${PERFOPT}                     \
${EXE} ${CMDOPT} &> log.${ICNT}

ICNT=10 # 1024
CMDOPT=$(echo "${ICNT} 100000")
${PERF} ${PERFOPT}                     \
${EXE} ${CMDOPT} &> log.${ICNT}

ICNT=14 # 4096
CMDOPT=$(echo "${ICNT} 10000")
${PERF} ${PERFOPT}                     \
${EXE} ${CMDOPT} &> log.${ICNT}

ICNT=18 # 1024 x 256 
CMDOPT=$(echo "${ICNT} 10000")
${PERF} ${PERFOPT}                     \
${EXE} ${CMDOPT} &> log.${ICNT}

ICNT=20 # 1024 x 1024  
CMDOPT=$(echo "${ICNT} 5000")
${PERF} ${PERFOPT}                     \
${EXE} ${CMDOPT} &> log.${ICNT}

ICNT=22 # 1024 x 1024 x 4
CMDOPT=$(echo "${ICNT} 2000")
${PERF} ${PERFOPT}                     \
${EXE} ${CMDOPT} &> log.${ICNT}

ICNT=24 # 1024 x 1024 x 16
CMDOPT=$(echo "${ICNT} 2000")
${PERF} ${PERFOPT}                     \
${EXE} ${CMDOPT} &> log.${ICNT}
date >> STAMP

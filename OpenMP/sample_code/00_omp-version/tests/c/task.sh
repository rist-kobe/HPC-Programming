#! /bin/bash

export OMP_NUM_THREADS=1
export OMP_DISPLAY_ENV=TRUE

echo -n "START: "
date
ulimit -s unlimited

COMPILER=gcc
${COMPILER} --version
uname -a
lscpu

BINDIR=../../src/c
EXE=${BINDIR}/run.x

${EXE} &> logfile

echo -n "END: " 
date

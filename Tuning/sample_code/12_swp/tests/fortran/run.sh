#!/bin/bash

echo -n "START: " 
date
ulimit -s unlimited

# Set load module (absolute path could be preferable)
EXE=$(echo "../../src/fortran/run.x")

COMPILER=gcc

${COMPILER} --version
uname -a
lscpu

# Run
NDIM=100
$EXE ${NDIM} > outfile.${NDIM}

NDIM=400
$EXE ${NDIM} > outfile.${NDIM}

NDIM=1600
$EXE ${NDIM} > outfile.${NDIM}

echo -n "END: " 
date

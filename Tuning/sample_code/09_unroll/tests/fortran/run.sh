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

# Set outer loop and inner loop length
# NOUTL * NINNL < 8192*8192
NOUTL=200
NINNL=200

# Run
$EXE $NOUTL $NINNL > outlist

echo -n "END: " 
date

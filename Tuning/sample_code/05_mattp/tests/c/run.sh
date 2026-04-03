#!/bin/bash

echo -n "START: "
date
ulimit -s unlimited

# Set load module (absolute path could be preferable)
EXE=$(echo "../../src/c/run.x")

COMPILER=gcc
${COMPILER} --version
uname -a
lscpu

# Set matrix row size (< 8192)
NROW=1000

# Set block size ( < NROW )
NBK=20

# Run
$EXE $NROW $NBK > outlist

echo -n "END: " 
date

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

# Run
$EXE > outfile

# Get profiler data 
# If not setting -pg in the compiler option, comment out the following two lines
sleep 10s
gprof $EXE > prof.out 

echo -n "END: " 
date

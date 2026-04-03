#!/bin/bash

echo -n "START: " 
date
ulimit -s unlimited

# Set load module (absolute path could be preferable)
EXE=$(echo "../../src/cpp/run.x")

COMPILER=g++

${COMPILER} --version
uname -a
lscpu

# Run
${EXE} &> outfile

echo -n "END: " 
date

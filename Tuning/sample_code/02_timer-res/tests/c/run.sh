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

echo -n "END: " 
date

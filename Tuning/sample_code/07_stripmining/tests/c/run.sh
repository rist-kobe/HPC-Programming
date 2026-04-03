#! /bin/bash

echo -n "START: "
date
ulimit -s unlimited

# Set load module (absolute path could be preferable)
BIN=../../src/c
EXE=${BIN}/run.x

COMPILER=gcc
${COMPILER} --version
uname -a
lscpu

### Run
{
${EXE} 3000 1000 8 
sleep 1s
${EXE} 3000 1000 64 
sleep 1s
${EXE} 3000 1000 128 
sleep 1s
${EXE} 3000 1000 256
} > output

echo -n "END: " 
date

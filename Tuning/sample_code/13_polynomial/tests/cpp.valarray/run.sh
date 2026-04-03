#!/bin/bash

echo -n "START: " 
date
ulimit -s unlimited

BINDIR=../../src/cpp.valarray
EXE=${BINDIR}/run.x

COMPILER=g++
${COMPILER} --version
uname -a
lscpu

# Run
${EXE} &> run.log

echo -n "END: " 
date

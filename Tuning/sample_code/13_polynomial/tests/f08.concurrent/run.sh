#!/bin/bash

echo -n "START: " 
date
ulimit -s unlimited

BINDIR=../../src/f08.concurrent
EXE=${BINDIR}/run.x

COMPILER=gfortran
${COMPILER} --version
uname -a
lscpu

# Run
${EXE} &> run.log

echo -n "END: " 
date

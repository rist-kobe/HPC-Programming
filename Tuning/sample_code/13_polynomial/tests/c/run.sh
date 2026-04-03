#!/bin/bash

echo -n "START: " 
date
ulimit -s unlimited

BINDIR=../../src/c
EXE=${BINDIR}/run.x

COMPILER=gcc
${COMPILER} --version
uname -a
lscpu

# Run
${EXE} &> run.log

echo -n "END: " 
date

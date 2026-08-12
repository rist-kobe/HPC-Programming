#!/bin/bash

echo -n "START: " 
date
ulimit -s unlimited

BINDIR=../../src/cpp.et14
EXE=${BINDIR}/run.x


# Run
${EXE} &> run.log

echo -n "END: " 
date

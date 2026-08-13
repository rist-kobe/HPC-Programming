#!/bin/bash

echo -n "START: " 
date
ulimit -s unlimited

BINDIR=../../src/cpp.oo
EXE=${BINDIR}/run.x


# Run
${EXE} &> run.log

echo -n "END: " 
date

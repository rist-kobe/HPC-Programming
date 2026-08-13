#!/bin/bash

echo -n "START: " 
date
ulimit -s unlimited

BINDIR=../../src/cpp.etp
EXE=${BINDIR}/run.x


# Run
${EXE} &> run.log

echo -n "END: " 
date

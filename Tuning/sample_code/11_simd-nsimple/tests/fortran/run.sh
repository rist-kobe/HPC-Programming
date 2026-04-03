#!/bin/bash

echo -n "START: " 
date
ulimit -s unlimited

BINDIR=../../src/fortran

COMPILER=gfortran
${COMPILER} --version
uname -a
lscpu

EXE=${BINDIR}/run_v.x
${EXE} &> run_v.log

EXE=${BINDIR}/run_nv.x
${EXE} &> run_nv.log

echo -n "END: " 
date

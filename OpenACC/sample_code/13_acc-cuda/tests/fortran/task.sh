#!/bin/bash

echo -n "START: " 
date
ulimit -s unlimited

COMPILER=nvfortran
${COMPILER} --version
#uname -a
#lscpu
nvidia-smi -L

EXE=../../src/fortran/run.x

NS=20
${EXE} ${NS} > outfile.${NS}

NS=40
${EXE} ${NS} > outfile.${NS}

NS=80
${EXE} ${NS} > outfile.${NS}

NS=100
${EXE} ${NS} > outfile.${NS}

NS=200
${EXE} ${NS} > outfile.${NS}

NS=400
${EXE} ${NS} > outfile.${NS}

NS=600
${EXE} ${NS} > outfile.${NS}

NS=800
${EXE} ${NS} > outfile.${NS}

NS=1000
${EXE} ${NS} > outfile.${NS}

NS=1200
${EXE} ${NS} > outfile.${NS}

NS=1400
${EXE} ${NS} > outfile.${NS}

NS=1600
${EXE} ${NS} > outfile.${NS}

#NS=1800
#${EXE} ${NS} > outfile.${NS}

#NS=2000
#${EXE} ${NS} > outfile.${NS}

#NS=4000
#${EXE} ${NS} > outfile.${NS}

echo -n "END: " 
date

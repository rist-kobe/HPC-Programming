#!/bin/bash

ulimit -s unlimited

echo "----------------------------------------"
nvidia-smi -L
echo " "

export OMP_NUM_THREADS=2
export OMP_TARGET_OFFLOAD=mandatory

EXE=../../src/fortran.omptarget/run.x
${EXE} &> out.log 

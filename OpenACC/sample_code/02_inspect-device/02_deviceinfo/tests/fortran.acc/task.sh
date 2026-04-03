#!/bin/bash

ulimit -s unlimited

echo "----------------------------------------"
nvidia-smi -L
echo " "

EXE=../../src/fortran.acc/run.x
${EXE} &> out.log 

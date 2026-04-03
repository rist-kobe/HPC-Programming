#!/bin/bash

ulimit -s unlimited

echo "----------------------------------------"
nvidia-smi -L
echo " "

EXE=../../src/cpp.acc/run.x
${EXE} &> out.log 

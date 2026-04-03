#!/bin/sh

ulimit -s unlimited

echo "=================================="
nvidia-smi -L
echo " "

echo "=================================="
nvidia-smi topo -m 
echo " "

EXE=../../src/cpp/run.x

NSIZE=10000
${EXE} ${NSIZE} &> out.log 

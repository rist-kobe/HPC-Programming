#!/bin/bash
ulimit -s unlimited

nvidia-smi topo -m 

EXE=../../src/run.x

ID=0
NSIZE=1000
${EXE} ${NSIZE} 10000 &> out.${ID}.log 

ID=1
NSIZE=10000
${EXE} ${NSIZE} 5000 &> out.${ID}.log 

ID=2
NSIZE=100000
${EXE} ${NSIZE} 5000 &> out.${ID}.log 

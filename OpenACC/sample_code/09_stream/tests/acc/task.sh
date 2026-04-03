#!/bin/bash

echo -n "START: " 
date
ulimit -s unlimited

EXE=../../src/acc/stream

# Automatically set
ID=0
VL=-1
${EXE} -1 ${VL} &> out.${ID}.log

# Explicitly control hierarchical parallelism
ID=1
VL=32
${EXE} -1 ${VL} &> out.${ID}.log

ID=2
VL=64
${EXE} -1 ${VL} &> out.${ID}.log

ID=3
VL=128
${EXE} -1 ${VL} &> out.${ID}.log

ID=4
VL=256
${EXE} -1 ${VL} &> out.${ID}.log

echo -n "END: " 
date

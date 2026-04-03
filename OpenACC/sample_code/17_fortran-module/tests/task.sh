#!/bin/bash

ulimit -s unlimited

nvidia-smi topo -m 

EXE=../../src/run.x

ID=0
NS=1000
${EXE} ${NS} 10000 &> out.${ID}.log

ID=1
NS=10000
${EXE} ${NS} 5000 &> out.${ID}.log

ID=2
NS=100000
${EXE} ${NS} 2000 &> out.${ID}.log


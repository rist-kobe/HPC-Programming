#! /bin/sh

ulimit -s unlimited

echo "----------------------------------------"
nvidia-smi -L
echo " "

EXE=../../src/cpp.cuda/run.x
${EXE} &> out.log

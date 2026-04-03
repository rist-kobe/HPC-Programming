#!/bin/bash

echo -n "START: " 
date
ulimit -s unlimited

# Set load module (absolute path could be preferable)
BIN=../../src/c
EXE=${BIN}/run.x

COMPILER=gcc
${COMPILER} --version
uname -a
lscpu

MACHINE=$(uname -a)

{
${EXE}

cat << EOF


Here is an example of a result in
  '${MACHINE}'
Look from the third line (00000020)

EOF

echo "$ hexdump -C -v output_native"
hexdump -C -v output_native 
echo " "
echo "$ hexdump -C -v output_swap"
hexdump -C -v output_swap

} > logfile

echo -n "END: " 
date

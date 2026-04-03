#!/bin/bash

echo -n "START: " 
date
ulimit -s unlimited

COMPILER=gfortran
${COMPILER} --version
uname -a
lscpu

MACHINE=$(uname -a)

{
cat << EOF
Do program with native option

EOF
BIN=../../src/fortran/native
EXE=${BIN}/run.x
${EXE}

cat << EOF


Here is an example of a result in
  '${MACHINE}'
Look from the third line (00000020)

EOF

echo "$ hexdump -C -v output"
hexdump -C -v output 

echo "   output file moves to output_native"
mv output output_native


cat << EOF
Do program with swap option

EOF
BIN=../../src/fortran/swap
EXE=${BIN}/run.x
${EXE}

cat << EOF


Here is an example of a result in
  '${MACHINE}'
Look from the third line (00000020)

EOF

echo "$ hexdump -C -v output"
hexdump -C -v output 

echo "   output file moves to output_swap"
mv output output_swap

} > logfile

echo -n "END: " 
date

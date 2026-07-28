#!/bin/bash

# GNU
module purge
module load gcc/9.3.1
compiler=gnu
dirname=${compiler}
mkdir -p ${dirname}
{
gcc --version
make COMPILER=${compiler}
mv -v *.x ${dirname}/
make COMPILER=${compiler} veryclean
} &> ${compiler}.log
mv ${compiler}.log ${dirname}

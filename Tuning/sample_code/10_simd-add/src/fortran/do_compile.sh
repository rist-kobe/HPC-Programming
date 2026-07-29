#!/bin/bash

# GNU
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

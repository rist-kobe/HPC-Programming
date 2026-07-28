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

# Intel oneAPI 2022.3.1
module purge
module load intel_oneapi/2022.3.1
compiler=intel
dirname=${compiler}
mkdir -p ${dirname}
{
ifx --version
make COMPILER=${compiler}
mv -v *.x ${dirname}/
mv -v *.yaml ${dirname}/
mv -v *.optrpt ${dirname}/
make COMPILER=${compiler} veryclean
} &> ${compiler}.log
mv ${compiler}.log ${dirname}

# Intel oneAPI 2022.3.1 (Classic)
#module purge
#module load intel_oneapi/2022.3.1
#compiler=intel-classic
#dirname=${compiler}
#mkdir -p ${dirname}
#{
#icx --version
#make COMPILER=${compiler}
#mv -v *.x ${dirname}/
#mv -v *.optrpt ${dirname}/
#make COMPILER=${compiler} veryclean
#} &> ${compiler}.log
#mv ${compiler}.log ${dirname}

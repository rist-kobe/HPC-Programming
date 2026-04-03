#!/bin/bash

CONFIGDIR=MAKEFILES

# GNU
module purge
module load gcc/9.3.1
surfix=gnu
dirname=${surfix}
mkdir -p ${dirname}
{
gcc --version
make -f ${CONFIGDIR}/Makefile.${surfix}
mv -v *.x ${dirname}/
make -f ${CONFIGDIR}/Makefile.${surfix} veryclean
} &> ${surfix}.log
mv ${surfix}.log ${dirname}

# Intel oneAPI 2022.3.1
module purge
module load intel_oneapi/2022.3.1
surfix=intel
dirname=${surfix}
mkdir -p ${dirname}
{
ifx --version
make -f ${CONFIGDIR}/Makefile.${surfix}
mv -v *.x ${dirname}/
mv -v *.yaml ${dirname}/
mv -v *.optrpt ${dirname}/
make -f ${CONFIGDIR}/Makefile.${surfix} veryclean
} &> ${surfix}.log
mv ${surfix}.log ${dirname}

# Intel oneAPI 2022.3.1
#module purge
#module load intel_oneapi/2022.3.1
#surfix=intel-classic
#dirname=${surfix}
#mkdir -p ${dirname}
#{
#icx --version
#make -f ${CONFIGDIR}/Makefile.${surfix}
#mv -v *.x ${dirname}/
#mv -v *.optrpt ${dirname}/
#make -f ${CONFIGDIR}/Makefile.${surfix} veryclean
#} &> ${surfix}.log
#mv ${surfix}.log ${dirname}

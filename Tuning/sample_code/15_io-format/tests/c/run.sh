#! /bin/bash

echo -n "START: " 
date
ulimit -s unlimited

# Set load module (absolute path could be preferable)
BINDIR=../../src/c
EXE=${BINDIR}/run.x

COMPILER=gcc
${COMPILER} --version
uname -a
lscpu

# array size along z-asix (<200)
NZARRAY=120

# logfile
LOGF=$(echo "logfile")

if [ -f ${LOGF} ] ; then
   mv -f ${LOGF} ${LOGF}.old
fi

echo "*Array size along z-axis is "${NZARRAY} > ${LOGF}

# with format
${EXE} 0 ${NZARRAY} >> ${LOGF}

# Delete output files:
#   Comment out if you want to keep
rm data*.out

sleep 10s

# without format
${EXE} 1 ${NZARRAY} >> ${LOGF}

# Delete output files:
#   Comment out if you want to keep
rm data*.out

echo -n "END: " 
date

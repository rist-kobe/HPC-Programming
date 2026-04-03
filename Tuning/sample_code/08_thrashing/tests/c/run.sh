#! /bin/bash 

echo -n "START: " 
date
ulimit -s unlimited

### Load module
BIN=../../src/c
EXE=${BIN}/run.x

COMPILER=gcc
${COMPILER} --version
uname -a
lscpu

## L1 data cache size per way (KiB)
# 32KiB, 8way
L1W1=512 #4KiB/8bytes = 4*1024/8=512
L1W2=$((L1W1*2))
L1W05=$((L1W1/2))

{
### Run 
NSTA=$((L1W05/5))
NEND=$((4*L1W05/5))
ND=$(((NEND-NSTA+1)/5))

for i in `seq ${NSTA} ${ND} ${NEND}`; do
   ${EXE} ${i} 
   sleep 1s 
done

${EXE} ${L1W05} 
sleep 1s

NSTA=$((6*L1W05/5))
NEND=$((9*L1W05/5))
ND=$(((NEND-NSTA+1)/5))

for i in `seq ${NSTA} ${ND} ${NEND}`; do
   ${EXE} ${i} 
   sleep 1s 
done

${EXE} ${L1W1} 
sleep 1s

NSTA=$((6*L1W1/5))
NEND=$((9*L1W1/5))
ND=$(((NEND-NSTA+1)/5))

for i in `seq ${NSTA} ${ND} ${NEND}`; do
   ${EXE} ${i} 
   sleep 1s 
done

${EXE} ${L1W2} 
} > output 

echo -n "END: " 
date
### End of script

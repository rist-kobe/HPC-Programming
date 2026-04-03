#! /bin/bash 
# You can add other parameters to control block size to nblst, such as 80.
function nblst () {
cat <<EOF
40
60
EOF
}

echo -n "START: " 
date
ulimit -s unlimited

COMPILER=icc 
${COMPILER} --version
uname -a
lscpu

### Load module
BINDIR=../../src/c
EXE=${BINDIR}/run.x

## Run
for i in $(nblst); do
   SUFFIXES="${i}x2"
   NANB=$((i*i)) # ~ NA x NB

   # if you want to specify membind in NUMA
   NUMARUN= #$(echo "numactl --cpunodebind=0 --localalloc ")

   # NA :block size for innermost loop
   # NB :block size for 2nd innermost loop
{
   NA=$((2*i))
   NB=$((NANB/NA))
   echo "NA=${NA}, NB=${NB}"
   ${NUMARUN} ${EXE} ${NA} ${NB}
 
   NB=${i}
   NA=${i}
   echo "NA=${NA}, NB=${NB}"
   ${NUMARUN} ${EXE} ${NA} ${NB}

   NA=$((i/2))
   NB=$((NANB/NA))
   echo "NA=${NA}, NB=${NB}"
   ${NUMARUN} ${EXE} ${NA} ${NB}
} &> outfile.${SUFFIXES}

  sleep 3s
done

echo -n "END: " 
date
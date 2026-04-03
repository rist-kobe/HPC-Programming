#! /bin/bash 
function ntlst () {
cat <<EOF
1
4
6
8
12
EOF
}

echo -n "START: " 
date
ulimit -s unlimited

COMPILER=g++
${COMPILER} --version
uname -a
lscpu

### Load module
BINDIR=../../src/cpp

### Run
export OMP_DYNAMIC=FALSE
export OMP_STACKSIZE=1G
export OMP_SCHEDULE="STATIC"
export OMP_DISPLAY_ENV=TRUE
export OMP_PLACES=cores
export OMP_PROC_BIND=close

EXE=${BINDIR}/run.x
echo "With critical construct"
for nt in `ntlst` ; do
  if echo ${nt} |grep -E -s -q "^#.*" ; then
      continue
  fi
  export OMP_NUM_THREADS=${nt}
  echo "Threads: ${nt}"
  ${EXE} 
done

# Comment out the following 10 lines if you create the executable file without critical construct.
#EXE=${BINDIR}/run_wo_critical.x
#echo "Without critical construct"
#for nt in `ntlst` ; do
#  if echo ${nt} |grep -E -s -q "^#.*" ; then
#      continue
#  fi
#  export OMP_NUM_THREADS=${nt}
#  echo "Threads: ${nt}"
#  ${EXE} 
#done

echo -n "END: "
date
### End of script

#! /bin/bash 
function ntlst () {
cat <<EOF
#1
4
#8
#12
EOF
}


echo -n "START: " 
date
ulimit -s unlimited

COMPILER=gcc
${COMPILER} --version
uname -a
lscpu

### Load module
BINDIR=../../src/c
EXE=${BINDIR}/run.x

### Run
export OMP_WAIT_POLICY=ACTIVE
export OMP_STACKSIZE=1G
export OMP_SCHEDULE="STATIC"
export OMP_DISPLAY_ENV=TRUE
export OMP_PLACES=cores
export OMP_PROC_BIND=close
for nt in `ntlst` ; do
  if echo ${nt} |grep -E -s -q "^#.*" ; then
      continue
  fi
  export OMP_NUM_THREADS=${nt}
  echo "Threads: ${nt}"
  ${EXE} 100
done

echo -n "END: " 
date

### End of script

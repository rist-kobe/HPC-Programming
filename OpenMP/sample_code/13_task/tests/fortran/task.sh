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

COMPILER=gfortran
${COMPILER} --version
uname -a
lscpu

### Load module
BINDIR=../../src/fortran
EXE=${BINDIR}/run.x

### Run
#export OMP_DYNAMIC=FALSE
export OMP_WAIT_POLICY=ACTIVE
export OMP_STACKSIZE=1G
#export OMP_SCHEDULE="STATIC"
export OMP_DISPLAY_ENV=TRUE
export OMP_PLACES=cores
export OMP_PROC_BIND=spread
for nt in `ntlst` ; do
  export OMP_NUM_THREADS=${nt}
  echo "Threads: ${nt}"
  ${EXE} 
done

echo -n "END: " 
date
### End of script

#! /bin/bash 
function ntlst () {
cat <<EOF
1
2
4
8
EOF
}

export OMP_DISPLAY_ENV=TRUE

echo -n "START: " 
date
ulimit -s unlimited

COMPILER=gfortran
${COMPILER} --version
uname -a
lscpu

BINDIR=../../src/fortran

EXE=${BINDIR}/run.x
echo "With masked construct"
for nt in `ntlst` ; do
  if echo ${nt} |grep -E -s -q "^#.*" ; then
      continue
  fi
  export OMP_NUM_THREADS=${nt}
  echo "Threads: ${nt}"
  ${EXE} 
done

# Uncomment the following 10 lines if you create the executable file without masked construct (-DNOT_USE_MASKED).
#EXE=${BINDIR}/run_wo_masked.x
#echo "Without masked construct"
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

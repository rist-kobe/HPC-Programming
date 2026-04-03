#!/bin/bash
function lmtype () {
cat <<EOF
cuda
#cudauvm
serial
openmp
#openmptarget
EOF
}
#module purge
#module load 


export TMPDIR=.

CWD=${PWD}
for i in $(lmtype); do
   if echo ${i} |grep -E -s -q "^#"; then
      continue
   fi

   if [ ${i} == "cuda" ] ; then
      vdflags=" "   
   elif [ ${i} == "cudauvm" ] ; then
      vdflags="-DUSE_CUDAUVM"   
   elif [ ${i} == "serial" ] ; then
      vdflags="-DUSE_SERIAL"   
   elif [ ${i} == "openmp" ] ; then
      vdflags="-DUSE_OPENMP"   
   elif [ ${i} == "openmptarget" ] ; then
      vdflags="-DUSE_OPENMPTARGET"   
   else
      echo "ERROR"
      exit
   fi

   objdir="obj_${i}"
   if [ ! -d ${objdir} ] ; then
      mkdir -p ${objdir}
   fi

   cd ${objdir}
   cp -v ../*.cpp .
   cp -v ../*.hpp .
   make -f ../Makefile DFLAGS="${vdflags}"  &> make.log
   rm *.cpp
   rm *.hpp
   mkdir -p results
   cp -v ../task.sh results/
   cd ${CWD}
done

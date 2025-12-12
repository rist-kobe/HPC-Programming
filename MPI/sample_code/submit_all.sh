#!/bin/bash
set -x
#
JOB_PARTITION=${1}
if [ -f ${HOME}/.Local/${JOB_PARTITION}/setpath.sh ]; then
  . ${HOME}/.Local/${JOB_PARTITION}/setpath.sh
fi
#
for i in `find ./ -name "[a-r,t-z]*" -type d`
do
  pushd ${i}
  echo "$ "
  echo "$ ls -1"
  ls -1
  echo "$ "
  echo "$ sbatch -p" ${JOB_PARTITION} -t 00:01:00 "exec.sh" ${i##*/}
  sbatch -p ${JOB_PARTITION} -t 00:01:00 exec.sh ${i##*/}
  echo "$ "
  echo "$ squeue"
  squeue
  echo "$ "
  popd
done
#
for i in `find ./ -name "s*" -type d`
do
  pushd ${i}
  echo "$ "
  echo "$ ls -1"
  ls -1
  echo "$ "
  echo "$ sbatch -p" ${JOB_PARTITION} -t 00:01:00 "exec.sh" ${i##*/}
  sbatch -p ${JOB_PARTITION} -t 00:01:00 exec.sh ${i##*/}
  echo "$ "
  echo "$ squeue"
  squeue
  echo "$ "
  popd
done
#

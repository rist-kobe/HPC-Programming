#!/bin/bash
set -x
#
rm -f *.stdout *.stderr
for i in $(find . -name "[a-z]*" -type d)
do
  find "${i}" -name "${i##*/}" -type f -executable -delete
done
find . -name "exec.sh_[1-9]*.*"  -delete
find . -name "*[0-9].txt" -delete
find . -name "*[0-9].dat" -delete
find . -name "*_m.dat" -delete
find . -name "*.tmp" -delete
#
bash submit_all.sh testq 1> submit_all.sh.stdout 2> submit_all.sh.stderr
#
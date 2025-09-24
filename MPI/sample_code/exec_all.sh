#!/bin/bash
set -x
#
rm *.stdout
rm *.stderr
for i in `find . -name "[a-z]*" -type d`
do
  find ${i} -name ${i##*/} -type f -executable -print -exec rm {} \;
done
find . -name "exec.sh_[1-9]*.*" -print -exec rm {} \;
find . -name "*[0-9].txt" -print -exec rm {} \;
find . -name "*[0-9].dat" -print -exec rm {} \;
find . -name "*_m.dat" -print -exec rm {} \;
find . -name "*.tmp" -print -exec rm {} \;
#
bash submit_all.sh testq 1> submit_all.sh.stdout 2> submit_all.sh.stderr
#

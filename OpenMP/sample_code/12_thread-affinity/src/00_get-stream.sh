#!/bin/bash 
function dllist () {
cat <<EOF
HISTORY.txt
LICENSE.txt
Makefile
READ.ME
TO_DO
mysecond.c
stream.c
stream.f
EOF
}
URL=https://www.cs.virginia.edu/stream/FTP/Code/
DLDIR=stream.v5

mkdir -p ${DLDIR}
cd ${DLDIR}
for dl in $(dllist) ; do
   wget ${URL}/${dl} 
done
date > STAMP
cd ..

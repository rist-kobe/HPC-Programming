#! /bin/bash

echo -n "START: " 
date
ulimit -s unlimited

# Set load module (absolute path could be preferable)
EXE=$(echo "../../src/fortran/run.x")


# Run
$EXE > outlist

echo -n "END: "
date

#!/bin/bash

echo -n "START: " 
date
ulimit -s unlimited

# Set load module (absolute path could be preferable)
EXE=$(echo "../../src/cpp/run.x")



# Run
$EXE > outfile

# Get profiler data 
# If setting -pg in the compiler option, uncomment the following two lines
#sleep 10s
#gprof $EXE > prof.out

echo -n "END: " 
date

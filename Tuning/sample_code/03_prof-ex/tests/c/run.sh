#!/bin/bash

echo -n "START: " 
date
ulimit -s unlimited

# Set load module (absolute path could be preferable)
EXE=$(echo "../../src/c/diffuse.x")


# Run
${EXE} 0 > diffuse.out

# Get profiler data 
sleep 10s
gprof -p ${EXE} > flat.out
echo
cat flat.out
echo
gprof -q ${EXE} > call.out
cat call.out
echo -n "END: " 
date

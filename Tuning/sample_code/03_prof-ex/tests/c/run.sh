#!/bin/bash

echo -n "START: " 
date
ulimit -s unlimited

# Set load module (absolute path could be preferable)
EXE=$(echo "../../src/c/diffuse.x")

# Remove any stale profiling data before run
rm -f gmon.out

# Run
${EXE} 0 > diffuse.out

# Get profiler data 
sleep 10s

# Check that gmon.out was produced (requires -pg build)
if [ ! -f gmon.out ]; then
    echo "Error: gmon.out was not generated. Was the executable built with -pg?" >&2
    exit 1
fi

echo
gprof -p ${EXE} gmon.out > flat.out
cat flat.out
echo
echo
echo
gprof -q ${EXE} gmon.out > call.out
cat call.out
echo -n "END: " 
date
echo

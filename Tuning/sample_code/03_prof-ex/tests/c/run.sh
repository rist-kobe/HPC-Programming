#!/bin/bash

echo -n "START: " 
date
ulimit -s unlimited

# Set load module (absolute path could be preferable)
EXE=$(echo "../../src/c/diffuse.x")

# Remove any stale profiling data and reports before run
rm -f gmon.out flat.out call.out

# Run
${EXE} 0 > diffuse.out

# Get profiler data
if [ ! -f gmon.out ]; then
    echo "gmon.out not found. Rebuild with 'make MODE=gprof' and re-run." >&2
else
    sleep 10s
    gprof -p ${EXE} gmon.out > flat.out
    gprof -q ${EXE} gmon.out > call.out
fi
echo -n "END: " 
date
echo

#!/bin/bash

echo -n "START: " 
date
ulimit -s unlimited

# Set load module (absolute path could be preferable)
EXE=$(echo "../../src/fortran/diffuse.x")


# Remove any stale profiling data and reports before run
rm -f gmon.out flat.out call.out

# Run
${EXE} 0 &> diffuse.out

# Get profiler data 
sleep 10s

# gmon.out is only produced when the executable is built with -pg.
# If it is missing, warn and skip the gprof steps instead of failing.
if [ ! -f gmon.out ]; then
    echo "Warning: gmon.out was not generated (executable not built with -pg?). Skipping gprof." >&2
else
    gprof -p ${EXE} gmon.out > flat.out

    gprof -q ${EXE} gmon.out > call.out
fi

echo -n "END: " 
date

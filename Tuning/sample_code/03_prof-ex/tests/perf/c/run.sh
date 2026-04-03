#!/bin/bash

echo -n "START: " 
date
ulimit -s unlimited

# Set load module (absolute path might be preferable)
EXE=$(echo "../../../src/c/diffuse.x")

COMPILER=gcc
${COMPILER} --version
uname -a
lscpu

# You can check what kinds of Hardware events are 
# analyzed in your own architecture typing 'perf list'.

#
# Without call graph
#

# Run with recording
#perf record --event=cpu-cycles \
#${EXE} 0 &> diffuse.out

# Dump report
#perf report --header --stdio --input=perf.data  &> perf.out

#
# With call graph
#
## With call graph
perf record --event=cpu-cycles --call-graph fp \
${EXE} 0 &> diffuse.out

# Dump report
perf report --header --stdio --input=perf.data --call-graph &> perf.out

echo -n "END: " 
date

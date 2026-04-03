#!/bin/bash

NTHREADS=1 bash run.sh
NTHREADS=2 TAFF=close bash run.sh
NTHREADS=4 TAFF=close bash run.sh
NTHREADS=8 TAFF=close bash run.sh

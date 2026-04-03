#!/bin/bash

NTHREADS=1 bash run.sh
NTHREADS=2 TAFF=spread bash run.sh
NTHREADS=4 TAFF=spread bash run.sh
NTHREADS=8 TAFF=spread bash run.sh

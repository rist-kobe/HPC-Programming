#! /bin/bash

INP=perf.data
TARGET=calc_force_diffuse

perf annotate -l --stdio --input=$INP --source --symbol=$TARGET 

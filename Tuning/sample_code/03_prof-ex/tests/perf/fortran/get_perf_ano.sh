#! /bin/bash

INP=perf.data
TARGET=calc_force_diffuse_

perf annotate -l --stdio --input=$INP --source --symbol=$TARGET 

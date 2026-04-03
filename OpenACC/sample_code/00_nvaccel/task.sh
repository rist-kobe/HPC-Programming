#!/bin/bash
# Copyright 2025 Research Organization for Information Science and Technology

# Please specify a setting to use NVIDIA HPC SDK
# module load [something]

ulimit -s unlimited

echo "----------------------------------------"
nvidia-smi -L
echo " "

echo "----------------------------------------"
nvc++ --version
echo " "

echo "----------------------------------------"
nvaccelinfo
echo " "

echo "----------------------------------------"
lscpu
echo " "

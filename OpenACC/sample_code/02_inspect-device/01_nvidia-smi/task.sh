#!/bin/bash
# Copyright 2025 Research Organization for Information Science and Technology

# Please specify a setting to use NVIDIA HPC SDK
# module load [something]

ulimit -s unlimited

echo "--------------------------------------------"
nvidia-smi -L

echo "--------------------------------------------"
nvidia-smi topo -m

echo "--------------------------------------------"
nvidia-smi --query

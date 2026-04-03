#!/bin/bash
NRANK=${OMPI_COMM_WORLD_RANK}
{
#hostname -I
hostname 
uname -a
nvidia-smi -L
numactl --show
$1
} > outlist.$2.${NRANK}

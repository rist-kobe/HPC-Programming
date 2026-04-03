# To inspect your GPU machine, using nvidia-smi
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: March 25th 2025

In the elementary level, we suggest that you skip this sample.

## Instruction: Run
1. Edit `task.sh` properly to use NVIDIA HPC SDK. 
2. Run
```shell
$ bash task.sh
```
The script is successfully run in :
  * Xeon(IceLake)+NVIDIA A100, with NVIDIA HPC SDK 23.11
  * AMD EPYC 9654+NVIDIA H100, with NVIDIA HPC SDK 24.1

## Exercise
1. Check the topology of your GPU machine (i.e., the outputs of `nvidia-smi topo -m`). How many GPUs are there? What about the network connection interface between GPUs?  What about CPU Affinity?

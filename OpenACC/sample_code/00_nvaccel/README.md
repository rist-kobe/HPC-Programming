# To inspect your GPU machine
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: March 25th 2025

## Instruction: Run
1. Edit `task.sh` properly to use NVIDIA HPC SDK. 
2. Run
```shell
$ bash task.sh
```
The script is successfully run in :
  * Xeon(icelake)+NVIDIA A100, with NVIDIA HPC SDK 23.11
  * AMD EPYC 9654+NVIDIA H100, with NVIDIA HPC SDK 24.1

## Exercise
1. Check the specification of your GPU, focusing on ID, Number of Streaming multiprocessors, Compute Capability (CC), Warp size, Global memory size, and L2 cache size.  
2. Compare the outputs to the results in sample `deviceinfo`. 

# Performance analysis of Jacobi method with NVTX (NVIDIA Tools Extension Library)
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: March 27th 2025

## Instruction: Compile and Run
1. Source codes are in `src`. Choose either `src/c` (C) or `src/fortran`. 

2. Change directory
```shell
$ cd src/c        # C
$ ls
acc1  acc1.um  acc2
```
```shell
$ cd src/fortran  # Fortran 
$ ls 
acc1  acc2
```
You can find:  
* `acc1`         : Baseline of implementation. This is a **bad** implementation from a performance point of view (Why?).
* `acc1.um`      : `acc1` with managed/unified memory (See Makefile). **DO NOT SET pinned in -gpu options.** (Only C)
* `acc2`         : an optimized version of `acc0` and `acc1`, on host-device data transfer.

3. Make
```shell
$ cd src/c/acc1
$ cp Makefile.nvhpc Makefile  
# Edit Makefile if necessary.
$ make
...
```
```shell
$ cd src/fortran/acc1
$ cp Makefile.nvhpc Makefile  
# Edit Makefile if necessary.
$ make
...
```
The code is successfully compiled by: 
  * Xeon(icelake)+NVIDIA A100, with NVIDIA HPC SDK 23.11
  * GH200, with NVIDIA HPC SDK 24.3
4. Run
The sample scripts are located in `tests/c` and `tests/fortran`. 
```shell
$ cd tests/c/acc1
$ bash task.sh 
...
```
```shell
$ cd tests/fortran/acc1
$ bash task.sh 
...
```
The results will be shown in `out.log` and `err.log` using the above `task.sh`. 

## Exercise
1. Check the measurement range in the source code specified by `nvtxRangeStartA` and `nvtxRangeEnd`.
2. Study the difference on performance between `acc1` and `acc2` using the summary shown in `out.log`.
3. (Advanced) Try to analyze the measured performance using `nsys-ui` in [NVIDIA nsight-systems](https://developer.nvidia.com/nsight-systems).
4. (Advanced) Compare the performance difference between `acc2` and `acc1.um` if the managed/unified memory (a.k.a HMM in Linux) is available in your GPU device. We note that you need to specify **unified** in `-gpu` option when you use the unified memory in GH200.  Collect the information on page faults in `nsys profile` (e.g., `--cuda-um-cpu-page-faults=true` and `--cuda-um-gpu-page-faults=true`).
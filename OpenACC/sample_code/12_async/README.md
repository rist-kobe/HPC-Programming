# Asynchronous execution of host and devices
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: March 27th 2025

## Instruction: Compile and Run
1. Source codes are in `src`. Choose either `src/c` (C) or `src/fortran`. 

2. Change directory
```shell
$ cd src/c        # C
$ ls
async  noasync
```
```shell
$ cd src/fortran  # Fortran 
$ ls 
async  noasync
```
You can find:  
* `async`        : an asynchronous implementation
* `noaysnc`      : implementation without any asynchronous settings.

3. Make
```shell
$ cd src/c/async
$ cp Makefile.nvhpc Makefile  
# Edit Makefile if necessary.
$ make
...
```
```shell
$ cd src/fortran/async
$ cp Makefile.nvhpc Makefile  
# Edit Makefile if necessary.
$ make
...
```
The code is successfully compiled by: 
  * Xeon(icelake)+NVIDIA A100, with NVIDIA HPC SDK 23.11
4. Run
The sample scripts are located in `tests/c` and `tests/fortran`. 
```shell
$ cd tests/c/async
$ bash task.sh 
...
```
```shell
$ cd tests/fortran/async
$ bash task.sh 
...
```
The results will be shown in `out.log` and `err.log` using the above `task.sh`. 

## Exercise
1. Check the measurement range in the source code specified by `nvtxRangeStartA` and `nvtxRangeEnd`.
2. Study the difference on performance between `async` and `noasync` using the summary shown in `out.log`.
3. (Advanced) Try to analyze the measured performance using `nsys-ui` in [NVIDIA nsight-systems](https://developer.nvidia.com/nsight-systems).

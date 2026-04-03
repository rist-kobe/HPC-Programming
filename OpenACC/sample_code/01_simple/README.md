# Simple examples
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: March 27th 2025

## Instruction: Compile and Run
1. Source codes are in `src`. Choose either `src/c` (C) or `src/fortran`. 
2. Change directory
```shell
$ cd src/c        # C      
$ cd src/fortran  # Fortran      
```
3. Make
```
$ cp Makefile.nvhpc24.1 Makefile
$ make
```
The code is successfully compiled by: 
  * Xeon(icelake)+NVIDIA A100, with NVIDIA HPC SDK 23.11  (only fortran)
  * AMD EPYC 9654+NVIDIA H100, with NVIDIA HPC SDK 24.1
  * GH200, with NVIDIA HPC SDK 24.3 (only fortran)
4. Run
The sample scripts are located in `tests/c` and `tests/fortran`.
```shell
$ cd tests/c
$ bash task.sh 
```
```shell
$ cd tests/fortran
$ bash task.sh 
```
The information of your GPU device will be shown in `out.log` in the above example of execution.

## Exercise
1. Check compiler's diagnosis message. If removing `-Minfo=accel` from the compiler options, how does the message change?
2. Check the output when setting the environment variables, `NVCOMPILER_ACC_TIME` and `NVCOMPILER_ACC_NOTIFY`.

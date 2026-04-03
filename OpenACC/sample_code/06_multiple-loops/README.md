# Compiler-based approach for multiple loops: Kernels vs. Parallel
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: March 26th 2025

## Instruction: Compile and Run
1. Source codes are in `src`. Choose either `src/c` (C) or `src/fortran`. 
2. Change directory
```shell
$ cd src/c        # C      
$ cd src/fortran  # Fortran      
```
3. Make
```
$ cd src/c
$ cp Makefile.nvhpc23.11 Makefile  
$ make
$ cd ../src/fortran
$ cp Makefile.nvhpc23.11 Makefile 
$ make
```
The code is successfully compiled by: 
  * Xeon(icelake)+NVIDIA A100, with NVIDIA HPC SDK 23.11
  * AMD EPYC 9654+NVIDIA H100, with NVIDIA HPC SDK 24.1
4. Run
The sample scripts are located in `tests/c` and `tests/fortran`. 
```shell
$ cd tests/c
$ bash task.sh 
$ cd tests/fortran
$ bash task.sh 
```
The results will be shown in `err.log` using the above `task.sh`. File `out.log` may be empty.

## Exercise
1. Check compiler's diagnosis message. 
2. Compare the message of kernels construct to the one of parallel construct without any loop constructs.
3. Compare the message of Fortran to the one of C. 

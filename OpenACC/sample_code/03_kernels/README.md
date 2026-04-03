# Kernels construct
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
$ cp Makefile.nvhpc23.11 Makefile  # Using OpenACC (and OpenMP)
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
The results will be shown in `out.log` and `err.log` using the above `task.sh`.

## Exercise
1. Check compiler's diagnosis message, focusing on the size of gang, the size of vector, and a way of data copy. Does the compiler automatically transfer data to a device if data construct/clause are nothing?
2. Deactivate use of OpenACC in compiler options (See, e.g., Makefile.nvhpc23.11.noacc).
3. (Advanced) Check `err.log`, in which the results related to `NVCOMPILER_ACC_TIME=1` are shown.
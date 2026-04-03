# Use of private clause
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
The results will be shown in `out.log` and `err.log` using the above `task.sh`. 

## Exercise
1. Check compiler's diagnosis message. How does the message change if explicitly setting a temporal scalar variable in private clause?
2. Compare the message of Fortran to the one of C. 
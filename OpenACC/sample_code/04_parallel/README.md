# Parallel construct
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: March 28th 2025

## Instruction: Compile and Run
1. Source codes are in `src`. Choose either `src/c` (C) or `src/fortran`. 
2. Change directory
```shell
$ cd src/c        # C      
$ cd src/fortran  # Fortran      
```
3. Make
```
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
The results will be shown in `out.log` using the above `task.sh`.

## Exercise
1. Check compiler's diagnosis message, focusing on the size of gang, the size of vector, and a way of data copy. 
2. As for data transfer of scalars, compare the message to the one in the case of kernels construct.
3. Check effects of present clause in compiler's diagnosis message. 
4. (Advanced) If your device can use managed memory/unified memory, try to use `config/Makefile.*.um`, in which `managed` is set as the compiler options. We note that for GH200 `magaged` should be changed to `unified`. 
5. (Advanced) If you can measure the information of **page faults**, try to get it. A straightforward way is to use `nsys profile` with ` --cuda-um-gpu-page-faults=true`. Are there any difference between the case of data directives and the case of managed/unified memory?
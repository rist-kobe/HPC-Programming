# Elementary matrix-free iterative solver: Jacobi method
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: March 26th 2025

## Instruction: Compile and Run
1. Source codes are in `src`. Choose either `src/c` (C) or `src/fortran`. 

2. Change directory
```shell
$ cd src/c        # C
$ ls
acc0  acc1  acc1.mp  acc1.um  acc2 
```
```shell
$ cd src/fortran  # Fortran 
$ ls 
acc0  acc1  acc1.collapse  acc1.mp  acc1.um  acc2
```
You can find:  
* `acc0`         : a primitive version. We keep it as reference.   
* `acc1`         : Baseline of implementation (`acc0` + collapse clause). This is a **bad** implementation from a performance point of view (Why?).
* `acc1.collapse`: Fortran only. adding independent and collapse clauses to `acc1`.
* `acc1.mp`      : host multi-thread version of `acc1` with **OpenACC** (See Makefile). 
* `acc1.um`      : `acc1` with managed/unified memory (See Makefile). **DO NOT SET pinned in -gpu options.**
* `acc2`         : an optimized version of `acc0` and `acc1` on host-device data transfer.

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
1. Compare the implementation of `acc1` to the one of `acc2`. 
2. Check the difference on performance between `acc1`, `acc1.mp`, and `acc2`.
3. (Advanced) Compare the implementation of `acc1` to the one of `acc1.um` if the managed/unified memory is available in your GPU device. We note that you need to specify **unified** in `-gpu` option when you use the unified memory in GH200. Check the difference on performance between `acc1`, `acc1.um`, and `acc2`. 
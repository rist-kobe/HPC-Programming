# An example of OpenACC with CUDA: Matrix-Matrix multiplication of dense matrices 
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: March 27th 2025

## Instruction: Compile and Run
1. Source codes are in `src`. Choose either `src/c` (C) or `src/fortran`. 

2. Change directory
```shell
$ cd src/c        # C
```
```shell
$ cd src/fortran  # Fortran 
```

* Five (four for Fortran) kinds of kernels are implemented. All of those calculate the matrix-matrix multiplication.
  * `simple`        : Simple implementation for CPU with a single-core and serial execution.
  * `simple_acc`    : OpenACC version of `simple` for GPU device.
  * `simple_acc_gv` : the same as `simple_acc`, but hierarchical parallelism with gang and vector clauses is applied.
  * `my_CUDA_impl`  : CUDA version of `simple` for GPU device. 
  * `DGEMM_cuBLAS`  : Directly using cuBLAS (Only C).

3. Make
```shell
$ cd src/c
$ cp config/Makefile.nvhpc24.1 Makefile  
# Edit Makefile if necessary.
$ make
...
```
```shell
$ cd src/fortran
$ cp config/Makefile.nvhpc24.1 Makefile  
# Edit Makefile if necessary.
$ make
...
```
The code is successfully compiled by: 
  * Xeon(icelake)+NVIDIA A100, with NVIDIA HPC SDK 23.11
  * AMD EPYC 9654+NVIDIA H100, with NVIDIA HPC SDK 24.1 
  * GH200, with NVIDIA HPC SDK 24.3
4. Run
The sample scripts are located in `tests/c` and `tests/fortran`. 
```shell
$ cd tests/c
$ bash task.sh 
...
```
```shell
$ cd tests/fortran
$ bash task.sh 
...
```
The results will be shown in `outfile.n`, in which `n` means the matrix dimension, using the above `task.sh`. 

## Exercise
1. Compare the implementations of matrix-matrix multiplication between `simple` and `simple_acc`. 
2. Examine how to use CUDA-based kernel within OpenACC. Which kinds of directives are used?
3. Examine the performance in `outfile.n` (ELP (=Elapsed time) and GFlop/s) when increasing the matrix dimension (`NSIZE`). What about the theoretical peak performance of floating-point operations with double-precision? Compare the theoretical value to the measured results.
4. (Advanced) Try OpenMP Target implementation (`src/c.omptarget`).

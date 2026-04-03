# Jacobi method with multi GPUs and MPI
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: May 13rd, 2025

## Important remark
* This sample is suitable for using OpenMPI.

## Instruction: Compile and Run
1. Source codes are in `src`. Currently, we have only C version (`src/c`).

2. Change directory
```shell
$ cd src/c        # C
$ ls
acc1  acc2.dptr  acc2.update  omptarget1  omptarget2.dptr  omptarget2.update
```
You can find:  
* `acc1`             : Baseline of implementation. Before calling MPI communication functions, the data on device will copy to host.
* `acc2.dptr`        : Using `host_data` construct with `use_device` clause; Passing the address in device memory to the host memory.
* `acc2.update`      : Using `update` construct.
* `omptarget1`       : (Experimental) OpenMP-Target implementation of `acc1`
* `omptarget2.dptr`  : (Experimental) OpenMP-Target implementation of `acc2.dptr`. Using `target data` construct with `use_device_ptr` clause.
* `omptarget2.update`: (Experimental) OpenMP-Target implementation of `acc2.update`. Using `target data update` construct.

3. Make
```shell
$ cd src/c/acc1
$ cp Makefile.nvhpc24.1-ompi Makefile
# Edit Makefile if necessary.
$ make
...
```
The code is successfully compiled by: 
  * Xeon(icelake)+NVIDIA A100, with NVIDIA HPC SDK 23.11 and OpenMPI 4.1.7 
  * AMD EPYC 9654+NVIDIA H100, with NVIDIA HPC SDK 24.1 and OpenMPI 5.0.2 (Only for OpenACC implementations)
4. Run
The sample scripts are located in `tests/c`.
```shell
$ cd tests/c/acc1
# Edit task.sh if necessary. 
# We note that the number of GPUs per node should be equal to the number of MPI tasks per node in this sample.
$ bash task.sh 
...
```
The results will be shown in `out.log` and `err.log` using the above `task.sh`. 

## Exercise
1. Compare the implementation of `acc1` to the other two implementations, `acc2.dptr` and `acc2.update`. In particular, check how to pass the data on device into MPI communication functions. 
2. Study the difference on performance between `acc1`, `acc2.dptr`, and `acc2.update`.
3. (Advanced) Compare OpenACC implementations to OpenMP-Target implementations.
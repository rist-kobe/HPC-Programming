# Ping-pong (Send-Recieve) benchmark 
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: April 2nd 2025

## Important remark
* This sample is suitable for using OpenMPI.
* The number of MPI tasks must be two. 

## Instruction: Compile and Run
1. Source codes are in `src`. Currently, we have only C version.

2. Change directory
```shell
$ cd src/acc
$ ls
acc  cpu  cuda
```
You can find:  
* `acc`       : OpenACC-based implementation of Ping-Pong. 
   * `std.HDcpy`       : Sending communication mode is standard. Host-to-Device (and vice versa) copy is explicitly performed when MPI communication is performed. 
   * `std.dptr`        : Sending communication mode is standard. Use of `host_data` construct with `use_device` clause is applied.
   * `sync.dptr`       : Sending communication mode is synchronous (i.e., Ssend-Recv). Use of `host_data` construct with `use_device` clause is applied.
* `cpu`       : The original CPU-based implementation of Ping-Pong. Never use GPUs.
   * `std`             : Sending communication mode is standard. 
   * `sync`            : Sending communication mode is synchronous (i.e., Ssend-Recv). 
* `cuda`      : CUDA-based implementation of Ping-Pong.
   * `std.HDcpy`       : Sending communication mode is standard. Host-to-Device (and vice versa) copy is explicitly performed when MPI communication is performed. 
   * `std.cuda-aware`  : Sending communication mode is standard. If CUDA-aware MPI is available, you can run it.
   * `sync.cuda-aware` : Sending communication mode is synchronous (i.e., Ssend-Recv). If CUDA-aware MPI is available, you can run it.
* `omptarget` : OpenMP-Target-based implementation of Ping-Pong. 
   * `std.HDcpy`       : Sending communication mode is standard. Host-to-Device (and vice versa) copy is explicitly performed when MPI communication is performed. 
   * `std.dptr`        : Sending communication mode is standard. Use of `target data` construct with `use_device_ptr` clause is applied.
   * `sync.dptr`       : Sending communication mode is synchronous (i.e., Ssend-Recv). Use of `target data` construct with `use_device_ptr` clause is applied.

3. Make
```shell
$ cd src/acc/std.HDcpy
# Edit Makefile if necessary.
$ make
...
```
The code is successfully compiled by: 
  * Xeon(icelake)+NVIDIA A100, with NVIDIA HPC SDK 23.11 and OpenMPI 4.1.7 
  * AMD EPYC 9654+NVIDIA H100, with NVIDIA HPC SDK 24.1 and OpenMPI 5.0.2
4. Run
The sample scripts are located in `tests/`.
```shell
$ cd tests/acc/std.HDcpy
# Edit task.sh if necessary. 
# We note that the number of GPUs per node must be equal to two in this sample.
$ bash task.sh 
...
```
The results will be shown in `out.log` and `err.log` using the above `task.sh`. 

## Exercise
1. Compare the implementation of `acc/std.HDcpy` to that of `acc/std.dptr`. In particular, check how to pass the data on device into MPI communication functions. 
2. Study the difference on performance between `acc/std.HDcpy` and `acc/std.dptr`.
3. (Advanced) Check ping-pong implementations with OpenMP Target. Compare those with OpenACC version.
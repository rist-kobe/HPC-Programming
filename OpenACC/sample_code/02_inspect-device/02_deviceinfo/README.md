# Create a program to get the information of your GPU device
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: March 26th 2025

In the elementary level, we suggest that you skip this sample.

## Instruction: Compile and Run
1. Source code is in `src`. Choose `src/cpp.acc` (C) or `src/fortran.acc` (Fortran). 
2. Change directory
```shell
$cd src/cpp.acc       
```
```shell
$cd src/fortran.acc       
```
3. Make
```
$ cp Makefile.nvhpc24.1 Makefile
$ make
```
The code is successfully compiled by: 
  * Xeon(icelake)+NVIDIA A100, with NVIDIA HPC SDK 23.11
  * AMD EPYC 9654+NVIDIA H100, with NVIDIA HPC SDK 24.1
4. Run
The sample scripts are located in `tests/cpp.acc` and `tests/fortran.acc`. 
```shell
$ cd tests/cpp.acc
$ bash task.sh 
```
```shell
$ cd tests/fortran.acc
$ bash task.sh 
```
The information of your GPU device will be shown in `out.log` in the above example of execution.

## Exercise
1. Check how many GPU devices are there in your machine. 
2. Check the version of OpenACC specification in your machine. 
3. Check the driver version of GPU and the total amount of device memory.
4. Compare the outputs to the results in sample `nvaccel`.
5. (Advanced) Try CUDA version in `src/cpp.cuda`.
6. (Advanced) Try OpenMP Target version in `src/cpp.omptarget` or `src/fortran.omptarget`.

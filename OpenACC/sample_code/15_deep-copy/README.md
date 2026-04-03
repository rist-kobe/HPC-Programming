# Deep copy: C++ class and Fortran derived type
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: March 30th 2025

## Instruction: Compile and Run
1. Source codes are in `src`. Choose either `src/cpp` (C++) or `src/fortran`. 

2. Change directory
```shell
$ cd src/cpp        # C++
```
```shell
$ cd src/fortran  # Fortran 
```

3. Make
```shell
$ cd src/c
# Edit Makefile if necessary.
$ make
...
```
```shell
$ cd src/fortran
# Edit Makefile if necessary.
$ make
...
```
The code is successfully compiled by: 
  * AMD EPYC 9654+NVIDIA H100, with NVIDIA HPC SDK 24.1 
4. Run
The sample scripts are located in `tests/cpp` and `tests/fortran`. 
```shell
$ cd tests/cpp
$ bash task.sh 
...
```
```shell
$ cd tests/fortra
$ bash task.sh 
...
```
The results will be shown in `out.log` using the above `task.sh`. 

## Reference
* [OpenACC Getting started guide](https://docs.nvidia.com/hpc-sdk/compilers/openacc-gs/index.html)
* [GPU programming in GDEP solutions (in Japanese)](https://gdep-sol.co.jp/gpu-programming/openacc-no8/)

# Fortran module with OpenACC: declare directives and routine directives
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: March 30th 2025

## Instruction: Compile and Run
1. Source codes are in `src`. This sample contains only Fortran source files.

2. Change directory
```shell
$ cd src 
```

3. Make
```shell
$ cd src
# Edit Makefile if necessary.
$ make
...
```
The code is successfully compiled by: 
  * AMD EPYC 9654+NVIDIA H100, with NVIDIA HPC SDK 24.1 
4. Run
The sample scripts are located in `tests/`. 
```shell
$ cd tests
$ bash task.sh 
...
```
The results will be shown in `out.n.log` using the above `task.sh`, in which n is an integer.

## Reference
* [OpenACC Getting started guide](https://docs.nvidia.com/hpc-sdk/compilers/openacc-gs/index.html)

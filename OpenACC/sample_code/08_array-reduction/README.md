# Reduction of arrays in OpenACC and OpenMP Target
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: March 27th 2025

## Instruction: Compile and Run
1. Source codes are in `src`. You can find:

* `c.acc`            : C with OpenACC (for device)
* `c.omp`            : C with OpenMP (for host)
* `c.omptarget`      : C with OpenMP Target (for device)
* `fortran.acc`      : Fortran with OpenACC (for device)
* `fortran.omp`      : Fortran with OpenMP (for host)
* `fortran.omptarget`: Fortran with OpenMP Target (for device)

2. Change directory
```shell
$ cd src/c.acc        # C with OpenACC      
$ cd src/fortran.acc  # Fortran with OpenACC   
```
3. Make
For example,
```shell
$ cd src/c.acc
# Edit Makefile if necessary
$ make
```

The code is successfully compiled by: 
  * Xeon(icelake)+NVIDIA A100, with NVIDIA HPC SDK 23.11
  * AMD EPYC 9654+NVIDIA H100, with NVIDIA HPC SDK 24.1

4. Run
The sample scripts are located in `tests/`. 
```shell
$ cd tests/c.acc
$ bash task.sh 
$ cd tests/fortran.acc
$ bash task.sh 
...
```
The results will be shown in `out.log` and `err.log` using the above `task.sh`. 

## Exercise
1. Check the outputs of the reduction of arrays. In this example, one uses operations on integer variables. Thus, the results of different kinds of implementation (OpenACC, OpenMP, and OpenMP Target) should be identical.

```shell
# Example of outputs (C)
a[0]=-28
a[1]=-20
a[2]=-12
a[3]=-4
a[4]=4
a[5]=12
a[6]=20
a[7]=28
```

```shell
# Example of outputs (Fortran)
a(1)=28
a(2)=20
a(3)=12
a(4)=4
a(5)=-4
a(6)=-12
a(7)=-20
a(8)=-28
```

# STREAM for GPU: OpenACC and OpenMP Target
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: April 2nd 2025

## Instruction: Compile and Run
1. Source codes are in `src/`. You can find:

* `acc`      : OpenACC version. 
* `omptarget`: OpenMP target version
* `cuda`: CUDA version. We use an external source code. For more detailed information, please read `src/cuda/README.md`.

2. Change directory
```shell
$ cd src/acc 
```
```shell
$ cd src/omptarget
```

3. Make
```
$ vi Makefile 
# Edit it if necessary.
$ make
```
The code is successfully compiled by: 
  * Xeon(icelake)+NVIDIA A100, with NVIDIA HPC SDK 23.11

4. Run
The sample scripts are located in `tests/acc` and `tests/omptarget`.
```shell
$ cd tests/acc # Number of array elements is 1<<26 (default).
$ bash task.sh 
```
The information of your GPU device will be shown in `out.[n]` with `n=0, 1, 2, 3, 4` in the above example of execution.
* `out.0.log`: Simple implementation (compiler-based approach)
* `out.1.log`: hierarchical parallelism. Vector length=32 (OpenACC), Number of threads for device=32 (OpenMP target)
* `out.2.log`: hierarchical parallelism. Vector length=64 (OpenACC), Number of threads for device=64 (OpenMP target)
* `out.3.log`: hierarchical parallelism. Vector length=128 (OpenACC), Number of threads for device=128 (OpenMP target)
* `out.4.log`: hierarchical parallelism. Vector length=256 (OpenACC), Number of threads for device=256 (OpenMP target)

## Exercise
1. Check the results of the compiler-based approach. Compare the result to that in CUDA version (See lec01).  
2. Check the outputs when changing vector length (OpenACC) or number of threads for device (OpenMP Target).
3. (Advanced) Understand how to implement hierarchical parallelism with OpenACC or OpenMP Target.

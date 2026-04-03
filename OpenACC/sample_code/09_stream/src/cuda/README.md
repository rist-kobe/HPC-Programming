# STREAM for GPU by CUDA
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: March 6th 2026

## Instruction: Compile and Run
1. Get source code in [cuda-stream](https://github.com/bcumming/cuda-stream.git). 
```shell
$ bash 00get_src.sh
$ ls
cuda-stream ... 
```
2. Apply patch and Change directory
```shell
$ patch -p 1 -d cuda-stream < patch.cuda-stream.1Feb2024
$ cd cuda-stream       
```
3. Make
```
$ vi Makefile # Please properly edit ARCH variable in Makefile for your device. The output of nvaccel is a clue.
$ make
$ ls
stream  ... # executable file of STREAM benchmark
```
The code is successfully compiled by: 
  * Xeon(icelake)+NVIDIA A100, with NVIDIA HPC SDK 23.11
  * AMD EPYC 9654+NVIDIA H100, with NVIDIA HPC SDK 24.1
4. Run
The sample scripts are located in `../tests/`.
```shell
$ cd ../tests/defaults # Number of array elements is 1<<26 (default).
$ bash task.sh 
```
The information of your GPU device will be shown in `out.[n]` with `n=32, 64, 128, 192, 256, 1024` (size of thread block) in the above example of execution.

## Exercise
1. Check the outputs when changing the size of thread block.
2. Check the size of L2 cache in your device. Compare the cache size to the total amount of the used memory in the program.

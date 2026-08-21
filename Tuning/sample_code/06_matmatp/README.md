# Blocking in matrix-matrix products on dense matrices
* Author:       Yukihiro Ota (yota@rist.or.jp)
* Last update:  26th Jan. 2024

## Instruction: Compile
1. Source code is saved in `src/`. Choose either fortran or c.
2. Change directory.
```
$ cd src/c # On C
```
3. Before compiling the code, set `NSIZE` (matrix dimension) in `main.F90` or `main.c`.  
  * Before running this sample, we recommend that you measure the memory latency in your machine using [LMbench](https://lmbench.sourceforge.net/). You can estimate which level of cache is almost equivalent to the main memory from a latency point of view.   
  * `LMbench` requires `libtirpc`. If the library and the related header file are absent in your machine, you need to obtain it. On RHEL8, the corresponding library is involved in `libtirpc-devel`.
4. Type `make` with your desired compiler setting.
```
$ make
```
The code is successfully compiled by
   * GNU (8.5.0) on x86-64 systems 


## Instruction: Run and do analyses
1. Sample scripts are stored in `tests/`. Choose either fortran or c.       
2. Change directory
```
$ cd tests/c
```
3. Run a job script, `run.sh`.
```
# One example
$ bash run.sh
# Another example
$ chmod 755 run.sh
$ ./run.sh
```
4. The results will be summarized in files, `output.40x2` and `output.60x2`. The suffixes, e.g., `40x2`, indicate the setting of the two input parameters, `NA` and `NB`; `40x2` means that `NA*NB ~ 40 KiB` and `NB = 2`.

## Exercise 
1. Check your CPU, about the number of processor cores and the number of sockets. The following Linux commands would be helpful.  
```
$ cat /proc/cpuinfo
$ numactl -H
```
2. Check the size of cache with different levels, such as L1, L2, and last-level cache (e.g., L3), in your machine. On Linux, typing `getconf -a`, you can find the corresponding data in `LEVEL2_CACHE_SIZE`, for example. Also, check whether a certain level of cache is shared between cores or not. 
3. Evaluate the cache size per core. Also, set `NSIZE` parameter. Please read the comment on the top of `main.F90` or `main.c`. 
4. Check the access pattern of arrays in `mykernel.F90` or `mykernel.c`.  
5. Examine the elapsed time in each of kernels (`mmp_simple`, `mmp_simple_blk`, and `mmp_lex_tp_blk`), varying the size of blocks. Your compiler may generate better code even in the case of `mmp_simple`. This is good news if so; it means that manual optimization is unnecessary. 

## Advanced topics
1. Use a matrix-matrix product routine in a well-tuned library, e.g., DGEMM in BLAS (such as OpenBLAS). Compare the results to those in the hand-made kernels.  

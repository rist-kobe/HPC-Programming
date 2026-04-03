# Loop blocking: transpose a matrix 
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: 23rd Jan., 2024 

## Instruction: Compile
1. Source code is stored in `src/`. Choose either fortran or c.
2. Change directory
```
$ cd src/c # On c
```
3. Make
```
$ make -f Makefile.gnu 
```
The code is successfully compiled by GNU compiler (8.5.0 and 12.2.0) in AMD EPYC 7642. 

## Instruction: Run and do a performance analysis
1. Sample files are stored in `tests/`. Choose either fortran or c.
2. Change directory
```
cd tests/c # On c
```
3. Run a job script, `run.sh`.
```
# One example
$ bash run.sh
# Another example
$ chmod 755 run.sh
$ ./run.sh
```
4. Output is summarized in `outlist`. Compare the runtime between **Matrix transpose (standard)** and **Matrix transpose (blocking)**. 

## Exercise
1. Examine the sizes of L1 and L2 (and last-level) cache in your machine. In Linux, you can obtain this information typing `cat /proc/cpuinfo`. The number in `cache size` would be an answer. When you have multi-core machines, the last-level (e.g. L3) cache is typically shared with several cores.  
2. Run jobs changing matrix row size with a fixed small block size. Look at `Req. memory` in `outlist` Carefully observe behaviors when `Req. memory` is set around the size of a cache (per core). 
3. Set matrix row size so that `Req. memory` is far beyond a cache size. Then, measure the runtime changing block size. 

## Advanced topics
1. Examine the size of cache line in your machine. In Linux, use of `getconf` is straightforward. Also, `cpuid` is quite useful. How many array elements are included in one cache line if an element of an array is double precision?
```
$ getconf LEVEL2_CACHE_LINESIZE
$ getconf LEVEL1_DCACHE_LINESIZE
$ getconf -a |grep -E "CACHE"
```
2. If you have a profiler to obtain hardware events, measure memory access throughput (or memory bandwidth), rather than runtime.
3. Try to measure memory latency using [LMbench](https://lmbench.sourceforge.net/). 
  * `LMbench` requires `libtirpc`. If the library and the related header file are absent in your machine, you need to obtain it. On RHEL8, the corresponding library is involved in `libtirpc-devel`.  
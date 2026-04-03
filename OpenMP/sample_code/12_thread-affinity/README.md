# Understanding thread affinity via STREAM
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: March 6th 2026

## Instruction: Compile and run

### 1. Get source code

In this directory, there is no source code. You need to obtain [STREAM benchmark](https://www.cs.virginia.edu/stream).

```shell
$ cd src
$ bash 00_get-stream.sh
$ ls 
$ stream.v5 ... # The directory name is specified in 00_get-stream.sh
$ patch -p 1 -d stream.v5 < patch.rist # Not necessary, but we recommend you apply our patch file to the original source code.
patching file stream.c
patching file stream.f
```

### 2. Set array size

* [STREAM](https://www.cs.virginia.edu/stream) is a benchmark to measure memory bandwidth (memory access throughput, bytes/sec).
* The memory size for the arrays needs to be larger than the size of the Last-level cache (LLC) in your machine.
* Set the array size in the following manner. 
  * As for FORTRAN (stream.f): Edit `PARAMETER n` in stream.f.
  * As for C (stream.c): Edit `STREAM_ARRAY_SIZE` in stream.c. Or, set `-DSTREAM_ARRAY_SIZE` in makefile.

### 3. Edit Makefile and Do make

You need to set your C compiler (e.g., `gcc` and `clang`) in `CC` and your Fortran compiler (e.g., `gfortran` and `flang`) in `FF`. Moreover, you need to set a compiler option to activate OpenMP in `CFLAGS` and `FFLAGS`, such as`-fopenmp` (in GPU and LLVM) although the option depends on sorts of compilers.

```shell
$ cd stream.v5
$ vi Makefile # if using vi.
$ make # compile
```

Examples of the settings in Makefile are shown:

```Makefile
# GNU  
CC = gcc
CFLAGS = -O3 -fopenmp -ftree-vectorize
FF = gfortran
FFLAGS = -O3 -fopenmp -ftree-vectorize
```

```Makefile
# LLVM
CC = clang
CFLAGS = -O3 -fopenmp -fvectorize
FF = flang
FFLAGS = -O3 -fopenmp -fvectorize
```

### 4. Run

The sample scripts are located in `tests/c` (for C) and `tests/fortran` (for Fortran).

```shell
$ cd tests/c/close # Thread affinity is "close".
$ bash task.sh 1> out.log 2> err.log
```

```shell
$ cd tests/c/spread # Thread affinity is "spread".
$ bash task.sh 1> out.log 2> err.log
```

```shell
$ cd tests/fortran/close # Thread affinity is "close".
$ bash task.sh 1> out.log 2> err.log
```

```shell
$ cd tests/fortran/spread # Thread affinity is "spread".
$ bash task.sh 1> out.log 2> err.log
```

## Exercise
1. Check `task.sh` and `run.sh` in `tests/c/close` (`tests/fortran/close`) and `tests/c/spread` (`tests/fortran/spread`). You can find that the thread affinity is set by `TAFF` variable (See `run.sh`). 
2. Run the STREAM benchmark. Check the behaviors of `Triadd`-based bandwidth, increasing the number of threads up to the maximum allowed number in your CPU. 
3. Compare the results in `close` to the ones in `spread`., from the viewpoint of NUMA structures. We suggest that you check the results of `numactl -H` before the measurements. See `check-numa` sample.
4. (Advanced) Consider whether the source code of `STREAM` satisfyies with the first-tough policy in Linux.

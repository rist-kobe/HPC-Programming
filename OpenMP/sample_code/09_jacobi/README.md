# Jacobi method 
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: March 16th, 2026

## Instruction: Compile and Run
1. Source code is in `src/`. Choose either fortran or c. We have several versions of source code.
  * `serial`         : Serial code
  * `serial_delcopy` : Serial code w/o redundant copy 
  * `omp1`           : An OpenMP implementation 
  * `omp2`           : Another OpenMP implementation
  * `omp3`           : A worse OpenMP implementation

2. Change directory
```
$ cd src/c/omp1 # For example
```

3. Make
```
$ make
```
The code is successfully compiled by GNU 8.5.0 and later.
  * GNU 8.5.1 in AMD EPYC 7643
  * GNU 9.3.1 in Xeon Gold 6126
  * Apple clang 15.0.0 in Mac M1 (Arm64) with libomp (LLVM 16.0.4) (only for c)
     * Please read `on-macos.md`.

4. Run
```
$ env OMP_NUM_THREADS=8 ./run.x
#Alternatively,
$ export OMP_NUM_THREADS=8
$ ./run.x
```
The sample scripts are located in `tests/c` (for C) and `tests/fortran` (for Fortran). You can use them.
```
$ cd tests/c
$ cd omp1
$ bash task.sh 1> out.log 2> err.log
$ cd ../omp2
$ bash task.sh 1> out.log 2> err.log
(...)
```
The performance information is summarized in `logfile`.


## Exercise
1. Run `serial` version and check what are hotspots.
2. Try to parallelize the `serial` code with OpenMP directives. You can find an answer in `omp1`, but please first try this issue by yourself.
3. Run your omp version and check the elapsed time in each interval.
4. Compare the performance of your code to `omp1` and `omp2` in the sample.
5. Consider why `omp3` is worse than the others.

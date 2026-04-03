# loop construct: triadd (and daxpy)
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: January 31st 2024

## Instruction: Compile and Run
1. Source code is in `src/`. Choose either fortran or c.
2. Change directory
```
$cd src/c/      # C
$cd src/fortran # Fortran
```
3. Make
```
$ make
```
The code is successfully compiled by: 
  * GNU 8.5.1 in AMD EPYC 7643
  * GNU 9.3.1 in Xeon Gold 6126
  * Intel oneAPI 2022.3.1 in AMD EPYC 7643
  * Apple clang 15.0.0 in Mac M1 (Arm64) with libomp (LLVM 16.0.4) (only for c)
     * Please read `on-macos.md`.
4. Run
```
$ env OMP_NUM_THREADS=4 ./run.x
# Alternatively,
$ export OMP_NUM_THREADS=4
$ ./run.x
```
The sample scripts are located in `tests/c` (for C) and `tests/fortran` (for Fortran). You can use them.
```
$ cd tests/c
$ bash task.sh 1> out.log 2> err.log
```
The performance information is summarized in `out.log` in the above example of execution.

## Exercise
1. Observe a behavior of elapsed time changing the number of threads.
2. Create a parallel version of `daxpy` in `mykernel` using do/for construct. If you treat OpenMP in the first time, I suggest that you explicitly use default clause.

## ATTENTION for C++
You may face on some compile errors about shared clause in C++, even though your code is perfect from a OpenMP-spec point of view. Several reasons may exist; your **old** compiler does not fully support the OpenMP spec. Alternatively, there are pretty complex issues about C++ scope.

Within YO's personal experience, the following URLs are useful for resolving and understanding issues in C/C++.
* [http://stackoverflow.com/questions/13199398/openmp-predetermined-shared-for-shared](http://stackoverflow.com/questions/13199398/openmp-predetermined-shared-for-shared)
* [http://stackoverflow.com/questions/4610656/why-is-class-member-variable-x-not-allowed-to-be-sharedx-in-openmp/5007203](http://stackoverflow.com/questions/4610656/why-is-class-member-variable-x-not-allowed-to-be-sharedx-in-openmp/5007203)
* [https://software.intel.com/content/www/us/en/develop/articles/32-openmp-traps-for-c-developers.html](https://software.intel.com/content/www/us/en/develop/articles/32-openmp-traps-for-c-developers.html)

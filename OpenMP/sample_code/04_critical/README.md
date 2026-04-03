# Example of critical construct: Update shared variable in parallel region
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: January 31st 2024 

## Instruction: Compile and Run
1. Source files are in `src/`. Choose C++ or Fortran. 
2. Change directory
```
$cd src/cpp     # C++
$cd src/fortran # Fortran
```
3. Make
```
$ make
```
The code is successfully compiled by GNU 8.5.0.
  * GNU 8.5.1 in AMD EPYC 7643
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
$ cd tests/cpp
$ bash task.sh 1> out.log 2> err.log
```

## Exercise
1. Check which of variables have shared attribute in OpenMP parallel region. In particular, you carefully consider variables in class (C++) or module (Fortran).
2. Consider why `critical` construct is needed in `main.cpp` or `main.F90`.
3. Remove `critical` construct with hint `FUNC` in `main.cpp` or `main.F90` and run the program. You can do this when setting `-DWO_CRITICAL` in Makefile. How does the result change depending on the number of threads? (The result must be independent of the number of threads!)

## Output (examples)
```
$ cat out.log
(...)
With critical construct
Threads: 1
17249
Threads: 4
17249
Threads: 6
17249
Threads: 8
17249
Threads: 12
17249
Without critical construct
Threads: 1
17249
Threads: 4
17249
Threads: 6
-482
Threads: 8
-53
Threads: 12
16913
```

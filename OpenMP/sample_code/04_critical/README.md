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

Three variants can be built by editing the commented macro in the Makefile (or passing it on the command line):
  * default: correct implementation protected by a `critical` construct.
  * `-DWO_CRITICAL`: wrong implementation without the `critical` construct.
  * `-DBETTER_IMPL`: better implementation; the work array is a local variable, so no `critical` construct is needed and an `atomic` construct protecting only the update of `s` is enough.
4. Run
```
$ env OMP_NUM_THREADS=4 ./run.x
# Alternatively,
$ export OMP_NUM_THREADS=4
$ ./run.x
```
The sample scripts are located in `tests/cpp` (for C++) and `tests/fortran` (for Fortran). You can use them.
```
$ cd tests/cpp
$ bash task.sh 1> out.log 2> err.log
```

## Why is critical needed?
The work array `u` used by `func` is a class member (C++) or a module variable (Fortran). When `func` is called from an OpenMP parallel region, `u` is **shared** among the threads, so concurrent calls to `func` race on `u`. The `critical` construct serializes the calls and thus avoids the race.

Better alternatives, demonstrated by the `-DBETTER_IMPL` variant, are:
  * Make the work array a local variable of the function (`func_local`), so the function has no shared state; then only the update of the shared variable `s` must be protected, and an `atomic` construct (or an OpenMP `reduction` clause) is enough.
  * In Fortran, another option unique to the module-variable situation is to declare `u` as `threadprivate`, giving each thread its own copy of `u`.

## Exercise
1. Check which of variables have shared attribute in OpenMP parallel region. In particular, you carefully consider variables in class (C++) or module (Fortran).
2. Consider why `critical` construct is needed in `main.cpp` or `main.F90`.
3. Remove `critical` construct with hint `FUNC` in `main.cpp` or `main.F90` and run the program. You can do this when setting `-DWO_CRITICAL` in Makefile. How does the result change depending on the number of threads? (The result must be independent of the number of threads!)
4. Build the better implementation when setting `-DBETTER_IMPL` in Makefile and run the program. Confirm that the result is independent of the number of threads even though no `critical` construct is used. Consider why the `atomic` construct is sufficient here.

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
Better implementation (no shared work array)
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
```

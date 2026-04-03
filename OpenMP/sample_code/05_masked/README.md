# masked construct
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: October 1st 2024

## IMPORTANT REMARK
`masked` construct is introduced in OpenMP 5.1. Please check the specification of the standard: [https://www.openmp.org/](https://www.openmp.org/). This construct is regarded as an extension of `master` construct.  

Please check your OpenMP version and the supported features before compiling the program. If your compiler support `masked` construct, please set `-DNOT_USE_MASKED` in the compiler's option.   

## Instruction: Compile and Run
1. Source files are in `src/`. Choose C or Fortran. 
2. Change directory
```
$cd src/c/      # C
$cd src/fortran # Fortran
```
3. Make 
```
# If using masked construct:
$ make  # Note: Makefile is equivalent to Makefile.gnu.
# If not using masked construct:
$ make DFLAGS=-DNOT_USE_MASKED
```
The code is successfully compiled by:
  * GNU (14.1.0 and 12.2.0; miniforge3 for x86) in AMD EPYC 7642/7643
  * GNU (8.5.0) in AMD EPYC 7642/7643, without using `masked` construct (i.e., setting `-DNOT_USE_MASKED`).
  * Intel (oneAPI 2022.3.1) in AMD EPYC 7642/7643 (x86)
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

## Exercise
1. Check the position of `barrier` constructs. Are they properly located in the code?
2. Consider why the memory is dynamically allocated taking the size of cache line into account (`malloc` in C and `allocate` in Fortran).
3. Change the integer in `filter` clause and observe the behaviors. Note that this integer must be less than the total number of threads.

## Output (examples)
```
# C
$ cat stdout.log
(...)
Thread 0: a[0] = 1
Thread 1: a[16] = 2
Thread 2: a[32] = 1
Thread 3: a[48] = 1
# Fortran
$ cat stdout.log 
(...)
Thread     0: a(    1) =          1
Thread     1: a(   17) =          2
Thread     2: a(   33) =          1
Thread     3: a(   49) =          1
```

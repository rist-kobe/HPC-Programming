# Hello World!
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: January 31st 2024

## Instruction: Compile and Run
1. Source files are in `src/`. Choose C or Fortran. 
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
  * Apple clang 15.0.0 in Mac M1 (Arm64) with libomp (LLVM 16.0.4) (only for c)
     * Please read `on-macos.md`.
4. Run
```
$ env OMP_NUM_THREADS=2 ./run.x
# Alternatively,
$ export OMP_NUM_THREADS=2
$ ./run.x
```
The sample scripts are located in `tests/c` (for C) and `tests/fortran` (for Fortran). You can use them.
```
$ cd tests/c
$ bash task.sh 1> out.log 2> err.log
```

## Exercise
1. Check a way of setting **"No use of OpenMP"** flag in your compiler. 
2. Consider why critical construct is needed.

## Output (examples)
```
$ cat out.log
(...)
Threads: 12
Hello.
Hello. I am thread-ID   0 of  12 threads
Hello. I am thread-ID   4 of  12 threads
Hello. I am thread-ID   5 of  12 threads
Hello. I am thread-ID   3 of  12 threads
Hello. I am thread-ID   1 of  12 threads
Hello. I am thread-ID   2 of  12 threads
Hello. I am thread-ID   7 of  12 threads
Hello. I am thread-ID   8 of  12 threads
Hello. I am thread-ID   6 of  12 threads
Hello. I am thread-ID  10 of  12 threads
Hello. I am thread-ID   9 of  12 threads
Hello. I am thread-ID  11 of  12 threads
```

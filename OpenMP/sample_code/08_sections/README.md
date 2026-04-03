# sections construct: Hello world
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: January 24th 2024

## Instruction: Compile and Run
1. Source code is in `src/`. Choose either fortran or c.
2. Change
```
$cd src/c/      # C
$cd src/fortran # Fortran
```
3. Make
```
$ make
```
The code is successfully compiled by GNU 8.5.0 and later.
  * GNU 8.5.1 in AMD EPYC 7643
  * GNU 9.3.1 in Xeon Gold 6126
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
1. Check the output changing the number of threads.

## Output (examples)
Only 4 threads are used except for `Threads: 1` since there are 4 `section` directives.
```
$ cat out.log
(...)
Threads: 1
Hello. I am funcA, with thread id 0
Hello. I am funcB, with thread id 0
Hello. I am funcC, with thread id 0
Hello. I am funcD, with thread id 0
Threads: 4
Hello. I am funcA, with thread id 0
Hello. I am funcC, with thread id 3
Hello. I am funcB, with thread id 1
Hello. I am funcD, with thread id 2
Threads: 8
Hello. I am funcB, with thread id 3
Hello. I am funcC, with thread id 2
Hello. I am funcD, with thread id 5
Hello. I am funcA, with thread id 1
Threads: 12
Hello. I am funcA, with thread id 0
Hello. I am funcC, with thread id 1
Hello. I am funcB, with thread id 3
Hello. I am funcD, with thread id 4
```

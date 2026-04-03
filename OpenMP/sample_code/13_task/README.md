# task construct: Hello world
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: January 24th 2024

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
Compare thread ID for queuing to ID for running tasks.
```
$ cat out.log
(...)
Threads: 1
I am 0. Hello world from a random thread.
task 1 run by tid=0, queued by 0
task 2 run by tid=0, queued by 0
task 3 run by tid=0, queued by 0
Threads: 4
I am 0. Hello world from a random thread.
task 1 run by tid=1, queued by 0
task 3 run by tid=2, queued by 0
task 2 run by tid=3, queued by 0
Threads: 6
I am 0. Hello world from a random thread.
task 1 run by tid=5, queued by 0
task 2 run by tid=3, queued by 0
task 3 run by tid=4, queued by 0
Threads: 8
I am 0. Hello world from a random thread.
task 1 run by tid=6, queued by 0
task 2 run by tid=2, queued by 0
task 3 run by tid=3, queued by 0
Threads: 12
I am 0. Hello world from a random thread.
task 1 run by tid=9, queued by 0
task 2 run by tid=10, queued by 0
task 3 run by tid=9, queued by 0
```

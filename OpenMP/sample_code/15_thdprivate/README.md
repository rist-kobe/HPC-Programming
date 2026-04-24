# threadprivate directive
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
$ export OMP_NUM_THREADS=4
$ ./run.x 100
```
The sample scripts are located in `tests/c` (for C) and `tests/fortran` (for Fortran). You can use them.
```
$ cd tests/c
$ bash task.sh 1> out.log 2> err.log
```

## Exercise
1. Check the output (i.e., the value of count) on each of threads. In particular, compare the results between round 1 and round 2.
2. In the intermediate stage between rounds 1 and 2, the value of count can be equal to the one on the master thread (thread ID=0). Explain the reason.

## Output (examples)
```
$ cat out.log
(...)
Threads: 4
Round 1: thread ID=2,  # of negative v: 0
Round 1: thread ID=0,  # of negative v: 25
Round 1: thread ID=1,  # of negative v: 25
Round 1: thread ID=3,  # of negative v: 0
b/w Rounds 1 and 2: primary thread, # of negative v:   25
Round 2: thread ID=2,  # of negative v: 0
Round 2: thread ID=3,  # of negative v: 0
Round 2: thread ID=1,  # of negative v: 50
Round 2: thread ID=0,  # of negative v: 50
```

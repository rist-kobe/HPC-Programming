# task construct: Fibonacci series 
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
1. Check the results between 1 thread (serial) and n thread (n>1).
2. What will happen if no `taskwait` construct?

## Output (examples)
```
$ cat out.log
(...)
Threads: 12
fibonacci_1 =   1
fibonacci_2 =   1
fibonacci_3 =   2
fibonacci_4 =   3
fibonacci_5 =   5
fibonacci_6 =   8
fibonacci_7 =  13
fibonacci_8 =  21
fibonacci_9 =  34
fibonacci_10 =  55
```

# reduction clause: On array
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: July 22nd 2026

The reduction loop is parallelized over the *second* index (`j`), so every thread accumulates into every element of the array `a`. This is precisely why an array reduction is required: each thread works on a private copy of the whole array, and OpenMP combines the private copies element-wise at the end of the loop.

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
1. Write the reduction calculation without using `reduction` clause. 
2. Parallelize the outer `i` loop instead of the `j` loop. Is the reduction clause still necessary? Why or why not?

## Output (examples)
```
$ cat out.log
(...)
Threads: 4
a[0]=-28
a[1]=-20
a[2]=-12
a[3]=-4
a[4]=4
a[5]=12
a[6]=20
a[7]=28
```

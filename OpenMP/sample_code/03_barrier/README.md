# barrier construct
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
Note: Number of threads in the source code is fixed as 4, with `num_threads` clause in parallel directive.   

The sample scripts are located in `tests/c` (for C) and `tests/fortran` (for Fortran). You can use them.
```
$ cd tests/c
$ bash task.sh 1> out.log 2> err.log
```

## Exercise
1. Check the value of array b, depending on the presence of `barrier` construct. We suggest that you run the program several times. 

## Output (examples)
When barrier` construct is dropped, the results of array `b` may change every execution. (It indicates that such an implementation is wrong!)
```
$ cat out.log
(...)
Round 1, Threads: 4
[With omp_barrier]
a[0] =    4, b[0] =   76
a[1] =   17, b[1] =  245
a[2] =   76, b[2] =   12
a[3] =  241, b[3] =   29
[Without omp_barrier]
a[0] =    4, b[0] =   76
a[1] =   17, b[1] =    4
a[2] =   76, b[2] =    8
a[3] =  241, b[3] =   29
Round 2, Threads: 4
[With omp_barrier]
a[0] =    4, b[0] =   76
a[1] =   17, b[1] =  245
a[2] =   76, b[2] =   12
a[3] =  241, b[3] =   29
[Without omp_barrier]
a[0] =    4, b[0] =   76
a[1] =   17, b[1] =  245
a[2] =   76, b[2] =    8
a[3] =  241, b[3] =   12
Round 3, Threads: 4
[With omp_barrier]
a[0] =    4, b[0] =   76
a[1] =   17, b[1] =  245
a[2] =   76, b[2] =   12
a[3] =  241, b[3] =   29
[Without omp_barrier]
a[0] =    4, b[0] =   76
a[1] =   17, b[1] =    4
a[2] =   76, b[2] =    8
a[3] =  241, b[3] =   29
```

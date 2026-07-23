# Jacobi method 
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: July 22nd, 2026

## Model
The code solves a 2-dim. Poisson equation discretized on a 1024 x 1024
grid with the Jacobi method: a point charge sits at the grid-center
cell and the potential `phi` obeys a Dirichlet boundary condition
(`phi = 0` on the whole boundary). Two buffers hold the current and
the next iterate; the iteration stops when the scaled residual
satisfies `|| A x - b ||^2 <= tol * || b ||^2`.

## Instruction: Compile and Run
1. Source code is in `src/`. Choose either fortran or c. We have several versions of source code.
  * `serial`         : Serial code. The new iterate is copied back to the current buffer in every iteration.
  * `serial_delcopy` : Serial code w/o the redundant copy. The two buffers are swapped through pointers instead of being copied, which removes an O(NX*NY) copy per iteration.
  * `omp1`           : An OpenMP implementation. Every loop is parallelized with its own `parallel for` (`parallel do`) construct.
  * `omp2`           : Another OpenMP implementation. A single parallel region encloses all the work-sharing loops, so the team of threads is forked only once per iteration (less fork-join overhead than `omp1`).
  * `omp3`           : A worse OpenMP implementation. See Exercise 5; the mechanism is explained in a comment at the offending directive in the code.

All versions place the point charge at the same grid-center cell
(`NX/2, NY/2` in the 0-based C indexing; `NX/2+1, NY/2+1` in the
1-based Fortran indexing) and produce the same result.

2. Change directory
```
$ cd src/c/omp1 # For example
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
$ ulimit -s unlimited
$ env OMP_NUM_THREADS=8 ./run.x
#Alternatively,
$ export OMP_NUM_THREADS=8
$ ./run.x
```
Note: the `serial` and `serial_delcopy` versions (and the OpenMP
versions when built with `-DUSE_STATICMEM`) keep about 24 MiB of
arrays on the stack, so `ulimit -s unlimited` is required. The
sample scripts below already set it.

The sample scripts are located in `tests/c` (for C) and `tests/fortran` (for Fortran). You can use them.
```
$ cd tests/c
$ cd omp1
$ bash task.sh 1> out.log 2> err.log
$ cd ../omp2
$ bash task.sh 1> out.log 2> err.log
(...)
```
The performance information is summarized in `logfile`.

## Output
Every version writes the converged potential to `phi.dat`
(`ix iy phi` triples, one block per grid line). You can visualize it
with gnuplot, e.g.
```
gnuplot> splot "phi.dat" with pm3d
```

## Exercise
1. Run `serial` version and check what are hotspots.
2. Try to parallelize the `serial` code with OpenMP directives. You can find an answer in `omp1`, but please first try this issue by yourself.
3. Run your omp version and check the elapsed time in each interval.
4. Compare the performance of your code to `omp1` and `omp2` in the sample.
5. Consider why `omp3` is worse than the others.
6. Convert the buffer-swap technique of `serial_delcopy` into an OpenMP version and compare its performance with `omp1` and `omp2`.
7. Try a `collapse(2)` clause on the stencil loop and check whether it changes the performance.
8. Experiment with different `schedule` kinds (`static`, `dynamic`, `guided`) and chunk sizes.

# schedule clause
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: January 31st 2024

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
The code is successfully compiled by:
  * GNU 8.5.1 in AMD EPYC 7643
  * GNU 9.3.1 in Xeon Gold 6126
  * Apple clang 15.0.0 in Mac M1 (Arm64) with libomp (LLVM 16.0.4) (only for c)
     * Please read [`misc/memo_omp_in_macos.md`](../misc/memo_omp_in_macos.md).
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
The performance information is summarized in `out.log` in the above example of execution.

## Notes on the output
* The number of threads (`omp_get_max_threads`) is printed at the beginning for self-documenting logs.
* A trivial warm-up `parallel` region is executed before the first timed parallel run, so that the thread-team creation overhead is not charged to the `OMP static` measurement.
* After each parallel run, the maximum absolute difference (`Max diff`) between the parallel result and the serial reference is printed. Changing the `schedule` clause must not change the results; only tiny rounding differences from the different summation orders in `reduction` are expected.
* The pair-force loop is separated from the main function into a kernel function/subroutine per scheduling variant (`force_serial`, `force_static`, `force_static1`, `force_dynamic1`, and `force_guided`), which also gives a clear per-region output in performance profilers (see Exercise 4).

## Exercise
1. Check the elapsed time on each of calculation settings, namely `Serial`, `OMP static`, `OMP static chunk 1`, `OMP dynamic chunk 1`, and `OMP guided`, changing the number of threads and array size (`NSIZE`).
2. Consider why the outermost loop should be a target for `do/fo` construct.
3. Consider why `fij` must be explicitly set in `private` clause. 
4. (Advanced) If you can use performance profiler for OpenMP, try to understand the speedup originating from schedule clause, from an imbalance point of view. In this exercise, the loop calculation is already separated from the main function into one kernel function per scheduling variant, so you can obtain a clear output per region. 
```
# Example: Use of VTune Amplifier (ver. 2018)
# [Step1] Collect the sampling information.
$ amplxe-cl -collect hotspots -result-dir=perf -knob analyze-openmp=true ./run.x
# [Step2] Dump a readable file for a report with imbalance information.
$ amplxe-cl -report hotspots -group-by=region -format=csv -report-output=rep.csv -r perf 
```

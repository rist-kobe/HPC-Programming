# Check resolution of timer 
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: 30th Jan., 2024 

## Instruction: Compile
1. Source code is stored in `src/`. Choose either fortran, c, or cpp. An optional setting, Fortran with c timer is in `src/fortran_c`.
2. Change directory
```
$ cd src/c # On c
```
3. Make
```
$ make -f Makefile.gnu # if using GNU compiler
$ make -f Makefile.intel # if using Intel compiler
$ make -f Makefile.clang # if using LLVM clang
```
The code is successfully compiled by
   * AOCC (3.2.0; LLVM 13.0.0) in AMD EPYC 7643 (x86)
   * GNU (8.5.0) in AMD EPYC 7642/7643 (x86)
   * GNU (9.3.1) in Intel Xeon Gold 6126
   * Intel (oneAPI 2022.3.1) in AMD EPYC 7642/7643 (x86)
   * Intel (oneAPI 2022.3.1) in Intel Xeon Gold 6126
   * Apple clang (15.0.0) in Mac M1 (arm64) (only for c and cpp)

## Instruction: Run and do a performance analysis
1. Sample scripts are stored in `tests/`. Choose either fortran or c.
2. Change directory
```
$ cd tests/c # On c
```
3. Run a job script, `run.sh`. 
```
# One example
$ bash run.sh
# Another example
$ chmod 755 run.sh
$ ./run.sh
```

## Exercise
1. As for C, compare the result to that obtained by `clock_getres` function.
2. Check the resolution of other timers, such as `omp_get_wtime` in OpenMP and `MPI_Wtime` in MPI, using similar idea.

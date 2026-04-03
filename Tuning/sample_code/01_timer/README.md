# By-hand timer and gprof 
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: 26th Jan., 2024 

## Instruction: Compile
1. Source code is stored in `src/`. Choose either fortran or c. 
  * Optional setting 1: Fortran with c timer is in `src/fortran_c`.
  * Optional setting 2: C++ with `std::chrono` is in `src/cpp`.
2. Change directory
```
$ cd src/c # On c
```
3. Make
```
$ make -f Makefile.clang # if using LLVM compiler (only C/C++)
$ make -f Makefile.gnu   # if using GNU compiler
$ make -f Makefile.intel # if using Intel compiler
```
The code is successfully compiled by
  * AOCC (3.2.0; LLVM 13.0.0) in AMD EPYC 7642/7643 (x86)
  * GNU (8.5.0) in AMD EPYC 7642/7643 (x86)
  * GNU (9.3.1) in Intel Xeon Gold 6126
  * Intel (oneAPI 2022.3.1) in AMD EPYC 7642/7643 (x86)
  * Intel (oneAPI 2022.3.1) in Intel Xeon Gold 6126

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
4. The result of `gprof` will be summarized in a file, `prof.out`

## Exercise
1. Find functions corresponding to a hotspot.
2. Change `gprof` mode to by-hand timer mode. In Makefile, you set `-DUSE_ELP_TIMER`, instead of using `-pg`. Then measure elapsed time. 
3. Also, you can try to measure CPU time, using `-DUSE_CPU_TIMER`. You may have to change array size to do a proper measurement.

# Check the version of OpenMP
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: October 1st 2024

## Instruction: Compile and Run
1. Source files are in `src/`. Choose C or Fortran. 
2. Change directory
```
$cd src/c/      # C
$cd src/fortran # Fortran
```
3. Make
```
$ make  # Note: Makefile is equivalent to Makefile.gnu.
```
The code is successfully compiled by:
  * AOCC (3.2.0; LLVM 13.0.0) in AMD EPYC 7642/7643 (x86)
  * GNU (8.5.0) in AMD EPYC 7642/7643 (x86)
  * GNU (9.3.1) in Intel Xeon Gold 6126
  * Intel (oneAPI 2022.3.1) in AMD EPYC 7642/7643 (x86)
  * Apple clang 15.0.0 in Mac M1 (Arm64) with libomp (LLVM 16.0.4) (only for c)
     * Please read `on-macos.md`.
4. Run
```
$ env OMP_NUM_THREADS=1 ./run.x
# Alternatively,
$ export OMP_NUM_THREADS=1
$ ./run.x
```
The sample scripts are located in `tests/c` (for C) and `tests/fortran` (for Fortran). You can use them.
```
$ cd tests/c
$ bash task.sh 
```
The file name of the output is `logfile`.

## Exercise
1. Check the version number of OpenMP in the output file. Then, compare the value to the issued date of the OpenMP standard, as seen in [https://www.openmp.org/](https://www.openmp.org/).
2. On Fortran: Examine the role of `!$` in the head of the statements. Checking whether the program is successfully compiled without an OpenMP activate flag (e.g., `-fopenmp`) will be helpful for understanding the role.

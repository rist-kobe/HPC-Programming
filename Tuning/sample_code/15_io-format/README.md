# Formatted (text) vs. Binary outputs   
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: 30th Jan., 2024 

## Instruction: Compile
1. Source code is in `src/`. Choose either fortran, c, or C++ 
2. Change directory
```
$ cd src/fortran  # Fortran
$ cd src/c        # C
$ cd src/cpp      # C++
```
3. Make
```
$ make -f Makefile.clang # if using LLVM compiler (only C/C++)
$ make -f Makefile.gnu   # if using GNU compiler
$ make -f Makefile.intel # if using Intel compiler
```
The code is successfully compiled by
  * AOCC (3.2.0; LLVM 13.0.0) in AMD EPYC 7642 (x86)
  * GNU (9.3.1) in Xeon Gold 6126 (x86)
  * GNU (8.5.0) in AMD EPYC 7642 (x86)
  * Intel (oneAPI 2022.3.1) in AMD EPYC 7642 (x86)
  * Apple clang (15.0.0) in Mac M1 (arm64) (only for c and cpp)

## Instruction: Run and do analyses
1. Sample scripts are in `tests/`. Choose either fortran, c, or c++.       
2. Change directory
```
cd tests/c # On c
```
3. Run a job script, `run.sh`.
```
# One example
$ bash run.sh
# Another example
$ chmod 755 run.sh
$ ./run.sh
```
4. The results will be summarized in file, `logfile`. 

## Exercise 
1. Compare measured elapsed times between the cases of text output (format=0 in `logfile`) and binary output (format=1 in `logfile`), changing array size along z-axis (=2nd arg. in command-line options).
   * You may think that CPU time should be almost zero when taking binary output. However, timing data does not always show this behavior. Indeed, CPU may perform something, such as processes associated with I/O and speculative execution. Eventually, it is difficult to completely distill I/O costs from elapsed time, and moreover the behaviors of CPU are quite complex in modern processors.
   * In conclusion, a concrete way of knowing costly part is to examine elapsed time (not CPU time) in specific part of a program, inserting timer routine. Alternatively, do function profiling of a program with wrapper functions of I/O routine, to recognize system functions and the standard library functions as user's own functions.

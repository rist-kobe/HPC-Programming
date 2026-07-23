# Example of SIMD: Vector addition
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: 30th Jan., 2024 

## Instruction: Compile
1. Source code is stored in `src/`. Choose either fortran or c.
2. Change directory
```
$ cd src/c # On c
```
3. Make
```
## For GNU
$ ln -s MAKEFILES/Makefile.gnu Makefile
$ make all
```
The code is successfully compiled by:  
  * AOCC (3.2.0; LLVM 13.0.0) in AMD EPYC 7642 (x86)
  * GNU (9.1.1) in Intel Xeon Gold 6126 (x86)
  * GNU (8.5.0) in AMD EPYC 7642 (x86)
  * Intel (oneAPI 2022.3.1) in Intel Xeon Gold 6126 (x86)
  * Intel (oneAPI 2022.3.1) in AMD EPYC 7642 (x86) 
  * Apple clang (15.0.0) in Mac M1 (arm64) without OpenMP (only c)
     * Remark: If you install `libomp`, you may use `-fopenmp` in Apple clang. Please see [OpenMP on macOS with Xcode tools](https://mac.r-project.org/openmp/).

## Instruction: Run and do a performance analysis
1. Sample scripts are stored in `tests/`. Choose either fortran or c.  
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
4. Outputs are summarized in `run_v.log` (for vectorized code) and `run_nv.log` (for non-vectorized code).  

## Exercise 
1. Check compiler's optimization reports on a vectorized code (`run_v.x`) and a non-vectorized code (`run_nv.x`). 
2. Is compiler-based auto-vectorization done, as your expected? Is auto-vectorization failed whenever the loop has a dependency? 

## Advanced topics
1. Check the assembly language files (`*.s`) for `mykernel.c`, depending on vectorization or non-vectorization. For example, typing `make mykernel_v.s` leads to the assembly language file for `mykernel.c` with auto-vectorization.
2. Check instructions relevant to vectorization in `*.s` files, looking at compiler's optimization reports.  

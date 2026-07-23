# Loop unrolling: on outer loop 
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
$ make -f Makefile.clang # if using LLVM compiler (only C)
$ make -f Makefile.gnu   # if using GNU compiler
$ make -f Makefile.intel # if using Intel compiler
```
The code is successfully compiled by 
  * AOCC (3.2.0; LLVM 13.0.0) in AMD EPYC 7642 (x86)
  * GNU (9.3.1) in Intel Xeon Gold 6126 (x86)
  * GNU (8.5.0) in AMD EPYC 7642 (x86)
  * Intel (oneAPI 2022.3.1) in Intel Xeon Gold 6126 (x86)
  * Intel (oneAPI 2022.3.1) in AMD EPYC 7642 (x86)
  * Apple clang (15.0.0) in Mac M1 (Arm64) (only for c)

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
4. Output is summarized in `outlist`. Compare the runtime between `fma0`, `fma4_nf`, and `fma4`. 

## Exercise
1. Consider difference between `fma4_nf` and `fma4`.  
2. Change optimization options in compiler. If your compiler has an option on automatic loop unrolling, let's try it.
3. Change the number of expansions in loop unrolling from 4 to 6, 8, etc. Then measure the performances.

## Advanced topics
1. Write an example on loop unrolling on inner or non-nested loops. Then, consider difference on roles from loop unrolling on outer loops.

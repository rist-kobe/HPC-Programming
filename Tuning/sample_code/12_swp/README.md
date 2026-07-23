# Example of software pipelining 
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: 16th Feb., 2024 

## Instruction: Compile
1. Source code is stored in `src/`. Choose either fortran or c. 
2. Change directory
```
$ cd src/c # On c
```
3. Make
```
$ make -f MAKEFILES/Makefile.gnu   # if using GNU compiler
$ make -f MAKEFILES/Makefile.intel # if using Intel compiler
$ make -f MAKEFILES/Makefile.clang # if using LLVM compiler (only C/C++)
```
The code is successfully compiled by
  * GNU (8.5.0) in AMD EPYC 7642/7643 (x86)
  * Intel (oneAPI 2022.3.1) in Intel Xeon Gold 6126
  * AOCC (3.2.0; LLVM 13.0.0) in AMD EPYC 7642/7643 (x86)
  * Apple clang (15.0.0) in MacM1 (arm64) (only for cpp, cpp.et14, cpp.oo, and cpp.valarray).

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
4. The results are summarized in `outfile.100`, `outfile.400`, and `outfile.1600`. The suffixes of the files indicate the length of innermost loops (=array size). 

## Exercise
1. Check the performance (i.e., elapsed time) of the original kernel (`cpmgs`) and the tuned kernel (`cpmgs_swp`), depending on the length of outetmost loops (`mvec` in the output files). 
2. Change compiler's optimization level. The original setting is `-O1`. You can change it into `-O2` or `-O3` or more aggressive ones.

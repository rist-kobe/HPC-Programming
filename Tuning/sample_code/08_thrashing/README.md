# Thrashing  
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: 30th Jan., 2024 

## Instruction: Compile
1. Source code is stored in `src/`. Choose either fortran or c.  
2. Change directory
```
$ cd src/c  # On C
```
3. Make
```
$ make -f MAKEFILES/Makefile.intel   # Intel compiler 
$ make -f MAKEFILES/Makefile.gnu     # GNU compiler 
$ make -f MAKEFILES/Makefile.clang   # LLVM compiler (only for C)
```
The code is successfully compiled by:
   * GNU (9.3.1) in Intel Xeon Gold 6126 
   * GNU (8.5.0) in AMD EPYC 7642/7643 (x86) 
   * Intel (oneAPI 2022.3.1) in Intel Xeon Gold 6126 
   * Intel (oneAPI 2022.3.1) in AMD EPYC 7642/7643 (x86)
   * AOCC (3.2.0; LLVM 13.0.0) in AMD EPYC 7642/7643 (x86)
   * Apple clang (15.0.0) in Mac M1 (arm64)

## Instruction: Run and do analyses
1. Sample scripts are stored in `tests/`. Choose either fortran or c.       
2. Change directory
```
$ cd tests/c
```
3. Run a job script, `run.sh`.
```
# One example
$ bash run.sh
# Another example
$ chmod 755 run.sh
$ ./run.sh
```
4. The results will be summarized in file, `output`. Column 1 is array size. Column 5 and Column l6 are measured timing results. Also, you can obtain the address of arrays in add.info file.

## Exercise
1. Check size of L1 data cache per way in your computer and set L1W in `run.sh` properly. On Unix/Linux, use of `getconf` command (`getconf -a |grep -E -i "cache"`) is a practical option for this purpose. As for Win, the use of `wmic` is plausible.  
2. Check the cpu time in the size competing with cache (i.e., `NSIZE = L1W` in `run.sh`). If cache associativity (i.e., number of ways) is larger, an effect of thrashing will be less. Eventually, you cannot find any abrupt change of the timing information around a specific value of `NSIZE` in your machine. This is good news!
  * When cache thrashing occurs, the cpu time suddenly increases in a specific array size. Typically, the cpu time is around 2 or 3 times longer than the values in the surrounding array sizes.  

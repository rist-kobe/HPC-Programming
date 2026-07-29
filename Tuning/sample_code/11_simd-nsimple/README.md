# Example of SIMD: Non-simple loops
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: 30th Jan.., 2024 

## Instruction: Compile
1. Source code is stored in `src/`. Choose either fortran or c.  
2. Change directory
```
$ cd src/c # On c
```
3. Make
```
## For GNU
$ make all                 # GNU compiler (default)
$ make COMPILER=gnu all    # explicit GNU selection
```
The code is successfully compiled by 
  * GNU (8.5.0) on x86-64 systems

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
2. Is compiler-based auto-vectorization done, as your expected? Consider AoS, SoA, and while-loop patterns, respectively.  

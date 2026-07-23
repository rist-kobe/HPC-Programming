# Check address of 2D arrays  
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: 30th Jan. 2024 

## Instruction: Compile
1. Source code is stored in `src/.` Choose either fortran or c. 
  * Optional: C++ code is in `src/cpp`. Fortran code with `ISO_C_BINDING` is in `src/fortran_cbind`.  
2. Change directory
```
$ cd src/c # On c
```
3. Make
```
$ make -f Makefile.gnu # if using GNU compiler
```
The code is successfully compiled by:
  * GNU compiler (9.3.1) in Intel Xeon Gold 6126.
  * GNU compiler (8.5.0) in AMD EPYC 7642/7643.  
  * Intel oneAPI 2022.3.1 in Intel Xeon Gold 6126.
     * **Unfixed**: Writing the address of a static array in `fortran_cbind` with Intel Fortran does not work, within my experiments. 
  * Apple clang (15.0.0) in Mac M1 (arm64). (only for c and cpp)

## Instruction: Run and do analyses
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
4. The results will be summarized in `outlist` file.  

## Exercise
1. Compare the results between Fortran90 and C. 
2. What about the results in dynamical allocation in C? Is the address of an array continuous?
3. Change type of arrays from `real(8)` (`double`) to `integer` (`int`) and observe the results.
4. Let us assume that the size of cache line is 64 bytes. Then, when accessing the first element of an array, how many do elements move from memory to cache?  

## Advanced topis
1. Try to dynamically allocate a contiguous 3D array in C/C++. One solution is shown in Sample `io-format` (`c/memory.c` and `cpp/memory.h`).

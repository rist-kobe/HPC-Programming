# Strip mining
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: 26th Jan., 2024 

## REMARK
* his sample should be skipped. A manual stripmining would be ineffective typically.

## Instruction: Compile
1. Source code is stored in `src/`. Choose either fortran or c.
2. Change directory
```
$ cd src/c # On c
```
3. Make
```
$ make -f Makefile.gnu # if using GNU compiler
$ make -f Makefile.intel # if using Intel compiler
$ make -f Makefile.intel-classic # if using Intel compiler(Classic)
```
The code is successfully compiled by
   * GNU (9.3.1) in Intel Xeon 6126
   * GNU (8.5.0) in AMD EPYC 7642/7643 (x86)
   * Intel (oneAPI 2022.3.1) in Intel Xeon 6126  
   * Intel (oneAPI 2022.3.1) in AMD EPYC 7642/7643 (x86)  

## Instruction: Run 
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

## Exercise
1. Change the 3rd argument of `run.x`, and check the elapsed time.  
2. Increase compiler's optimization level. The initial setting is `-O1`. How is the performance changed?

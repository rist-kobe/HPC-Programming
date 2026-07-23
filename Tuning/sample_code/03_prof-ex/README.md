# Exercise on a performance analysis with gprof (and perf)
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: 26th Jan. 2024

## Preparation
First, you need to obtain Mersenne-Twister source code for a random-number generator. This sample **DOES NOT** include it.
### Fortran
* Visit the following website and obtain `mtfortf90.f`: [http://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/VERSIONS/FORTRAN/fortran.html](http://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/VERSIONS/FORTRAN/fortran.html)
* Extract module part, `mtmod`, from `mtfortf90.f`. Then the resultant code is named by `mtfortf90.f90`.
* Copy `mtfortf90.f90` in `src/fortran`.
### C
* Visit the following website and obtain `mt19937ar.c` and `mt19937ar.h`: [http://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/mt.html](http://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/mt.html) and [http://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/MT2002/mt19937ar.html](http://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/MT2002/mt19937ar.html). Use of `mt19937ar.sep.tgz` is recommended.
* Copy `mt19937ar.c` and `mt19937ar.h` in `src/c`.
### On Mersenne-Twister 
* M. Matsumoto and T. Nishimura, "Mersenne Twister: A 623-Dimensionally Equidistributed Uniform Pseudo-Random Number Generator", ACM Transactions on Modeling and Computer Simulation, Vol. 8, No. 1, January 1998, pp 3-30.
* [Mersenne Twister Home Page](http://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/mt.html)

## Instruction: Compile
1. Source code is stored in `src/`. Choose either fortran or c.
2. Change directory
```
$ cd src/c # On c
```
3. Make
```
$ make -f Makefile.gnu # if using GNU compiler
```
The code is successfully compiled by:
   * GNU (8.5.0) in AMD EPYC 7642/7643 (x86)
   * GNU (9.3.1) in Intel Xeon Gold 6126

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
4. The result of `gprof` will be summarized in files if setting `-pg` in compiler's options.
5. You can also try to use another profiler, `perf`, according to the information form Linux kernel. You move to directory `tests/perf`.

## Exercise
1. Find functions corresponding to hotspot. 
2. Change the optimization level or a kind of compiler. What about the results? 
3. Make a timer routine. Insert it in any place of the source code.  Then, measure elapsed time. Also, find which lines in the hotspot functions have high runtime costs.  
4. Visualization: This is NOT related to performance tuning. You can visualize the resultant data, `trj.xyz`, with `Jmol`, for example: [http://jmol.sourceforge.net](http://jmol.sourceforge.net)
 
## Advanced topics
1. Consider how to optimize this code, according to the performance analysis and compiler's report.
2. How do you evaluate the used memory in runtime? A simple way on a Linux system is to read `/proc/self/status`. 
3. Measure FLOP/s and memory band width in a specific (small) part of this code, by hand. Compare them to their ideal values.  To know an effective memory bandwidth, use of `STREAM` would be useful: [http://www.cs.virginia.edu/stream](http://www.cs.virginia.edu/stream)
4. Understanding **bottleneck** is often important: Why is your program is so slow? In this case, a performance analysis model would be useful. Try the Roofline model. What kind of knowledge can you obtain via this approach?  
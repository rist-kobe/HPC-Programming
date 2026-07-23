# Comparison between different implementations of a compute-bound kernel, n-th order polynomial
* Author:       Yukihiro Ota (yota@rist.or.jp)
* Last update:  30th Jan., 2024

## Directory tree
```
src/
   + c/ 
   |           low-level c code
   + cpp/  
   |           c-like low-level code with C++
   + cpp.et14/
   |           Expression templates with C++14 standard.
   + cpp.etp/
   |           Expression templates with C++11 standard, in a primitive manner 
   + cpp.oo/
   |           Typical object-oriented implementation
   + cpp.valarray/
   |           Use of STL, valarray
   + f90/
   |           low-level fortran code
   + f90.forall/
   |           same as f90, but use of forall statement 
   + f08.concurrent/
   |           same as f90, but use of do concurrent construct in Fortran2008
```

## How to compile
1. As an example, in the case of `cpp/` we explain a way of compiling the program. Change directory
```
$ cd cpp/
```
2. Make 
```
## For GNU
$ ln -s MAKEFILES/Makefile.gnu Makefile
$ make 
```
The code is successfully compiled by
  * AOCC (3.2.0; LLVM 13.0.0) in AMD EPYC 7642 (x86)
  * GNU (8.5.0) in AMD EPYC 7642 (x86)
  * Intel (oneAPI 2022.3.1) in AMD EPYC 7642 (x86)
  * Fujitsu (4.10.0) in Fugaku (aarch64)
  * Fujitsu (4.10.0) in Wisteria/BDEC-01(Odyssey) (aarch64).
  * Apple clang (15.0.0) in MacM1 (arm64) (only for cpp, cpp.et14, cpp.oo, and cpp.valarray).

## Description
All of them calculate a completely identical kernel composed of several multiplication-and-add, but the implementations are different. As a result, the performance could be different. If doing the calculations, you can find `GFLOP/s` in the standard output. Compare such statistical information b/w different kinds of implementation.  

## Exercise 
1.  Compare `GFLOP/s` b/w different kinds of implementation. An expected result on performance (`FLOP/s`) is: `c = cpp = f90 = f90.forall = f08.concurrent = cpp.et14 = cpp.ept = cpp.valarray >> cpp.oo`. Depending on a kind of compiler, the above rank may change.
2. Why are the kenerls considered to be **compute-bound** one?

## Reference 
* Yukihiro Ota, Tasunobu Kokubo, and Takaaki Noguchi, "A Performance Analysis of Evaluating Polynomials with 
Expression Templates in Supercomputer K", HPCI Research Report, hp130038 [http://www.hpci-office.jp/annex/resrep/](http://www.hpci-office.jp/annex/resrep/) (and references therein). 

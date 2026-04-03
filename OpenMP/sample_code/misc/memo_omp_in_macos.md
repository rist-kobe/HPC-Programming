# Use of OpenMP in MacOS
* Yukihiro Ota (RIST)
* Last update: Mar. 6th, 2026

## You have two ways.

* Xcode tools
  * We explain this way here.
* [miniforge](https://github.com/conda-forge/miniforge) with conda or mamba 
  * Very simple. 
  * As for compilers in miniforge, use of LLVM may be preferable. Please use `conda install -c conda-forge c-compiler`, `conda install -c conda-forge cxx-compiler`, and `conda install -c conda-forge fortran-compiler`. Of course, You can replace `conda` with `mamba`.

## Use of Xcode tools

### The most important thing
* Apart from anything else, you need to carefully read [OpenMP on macOS with Xcode tools](https://mac.r-project.org/openmp/) first of all.
* Can you understand that everything is experimental? 
* Are you okay? If YES, try the following instruction to use OpenMP in MacOS with Apple clang.

### Testing environment
* Apple clang 15.0.0 in Mac M1 mini (Arm64).

### Instruction
1. Obtain the binary of libomp.
```
$ mkdir -p [install_dir]
$ curl -O https://mac.r-project.org/openmp/openmp-16.0.4-darwin20-Release.tar.gz
$ tar xzvf openmp-16.0.4-darwin20-Release.tar.gz -C [install_dir]
$ ls [install_dir]/usr/local
```

2. Setting for compiler
```
$ clang -Xclang -fopenmp -I[install_dir]/usr/local/include (compiler options) foo1.c -o foo1.o # compile
$ clang -Xclang -fopenmp -I[install_dir]/usr/local/include (compiler options) foo2.c -o foo2.o # compile
$ clang -Xclang -fopenmp (link options) foo1.o foo2.o -L[install_dir]/usr/local/lib -lomp      # link
```

3. Execution (4 threads, for example)
```
export DYLD_LIBRARY_PATH=[install_dir]/usr/local/lib:${DYLD_LIBRARY_PATH}
export OMP_NUM_THREADS=4
export OMP_DISPLAY_ENV=TRUE
./a.out
```

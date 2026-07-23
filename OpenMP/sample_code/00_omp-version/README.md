# Check the version of OpenMP
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: October 1st 2024

## Instruction: Compile and Run
1. Source files are in `src/`. Choose C or Fortran. 
2. Change directory
   ```
   $cd src/c/      # C
   $cd src/fortran # Fortran
   ```
3. Make
   ```
   $ make
   ```
   The code has been successfully compiled with the following configurations:
     * GNU (8.5.0) in AMD EPYC 7642/7643 (x86)

   To use a different compiler, override `CC` (C) or `FC` (Fortran) when running `make`, e.g., `make CC=icx` or `make FC=ifx`.
4. Run
   ```
   $ env OMP_NUM_THREADS=1 ./run.x
   # Alternatively,
   $ export OMP_NUM_THREADS=1
   $ ./run.x
   ```
   The sample scripts are located in `tests/c` (for C) and `tests/fortran` (for Fortran). You can use them.
   ```
   $ cd tests/c
   $ bash task.sh 
   ```
   The file name of the output is `logfile`.

## Exercise
1. Check the version number of OpenMP in the output file. Then, compare the value to the issued date of the OpenMP standard, as seen in [https://www.openmp.org/](https://www.openmp.org/).
2. On Fortran: Examine the role of `!$` at the start of statements. Checking whether the program compiles without an OpenMP activation flag (e.g., `-fopenmp`) will be helpful for understanding compiling code with OpenMP.

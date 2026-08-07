# Timer Sample: By-hand Timers and gprof
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: 26th Jan., 2024

## Purpose
This sample demonstrates three basic techniques for measuring the performance of a program, which are the first step of any tuning work:

1. **Elapsed (wall-clock) time measurement** with a by-hand timer inserted in the source code
2. **CPU time measurement** with a by-hand timer
3. **Profiling with `gprof`** to find hotspots without modifying the source code

The sample program (`main.c` / `main.f90`) calls three subroutines (`sub1`, `sub2`, `sub3`) with different call counts and workloads. By timing and profiling them, you will learn how to identify which routine dominates the execution time.

## Directory layout
```
01_timer/
├── src/            # Source code and Makefiles
│   ├── c/          # C version (default)
│   ├── fortran/    # Fortran version
│   ├── fortran_c/  # Optional: Fortran with a C timer
│   └── cpp/        # Optional: C++ with std::chrono
└── tests/          # Job scripts (run.sh) for each language
    ├── c/  ├── fortran/  ├── fortran_c/  └── cpp/
```

Choose either C or Fortran. The examples below use C; for Fortran, replace `src/c` and `tests/c` with `src/fortran` and `tests/fortran`.

The timer mode is selected by the compiler flags in the `Makefile` (`src/<lang>/Makefile`):

| Mode | Flag setting in Makefile |
|---|---|
| Elapsed time (wall clock) | `-DUSE_ELP_TIMER` (default) |
| CPU time | `-DUSE_CPU_TIMER` |
| gprof profiling | `-pg` (no `-DUSE_*_TIMER`) |

The code has been verified with GNU compilers (8.5.0) on x86-64 systems.
If linking fails, try `LIB=-lm -lrt` in the Makefile.

## Exercise steps

### Step 1: Measure elapsed (wall-clock) time
1. Move to the source directory and build. The default Makefile already sets `-DUSE_ELP_TIMER`:
   ```
   $ cd src/c
   $ make
   ```
2. Move to the test directory and run the job script:
   ```
   $ cd ../../tests/c
   $ bash run.sh
   ```
3. Check `outfile`. The elapsed time of each timed section is printed as:
   ```
   Elapsed time (sec) = ...
   ```
4. Compare the elapsed times of the two timed loops (routine 1 calling `sub1`, and routine 2 calling `sub2`) and consider which one is more expensive and why.

### Step 2: Measure CPU time
1. Go back to `src/c` and edit the `Makefile`: comment out the `-DUSE_ELP_TIMER` line and enable the `-DUSE_CPU_TIMER` line:
   ```makefile
   ## Use of timer for Elapsed time
   #CFLAGS=-g -Wall -O0 -std=gnu99 -DUSE_ELP_TIMER
   ## Use of timer for CPU time
   CFLAGS=-g -Wall -O0 -std=gnu99 -DUSE_CPU_TIMER
   ```
2. Rebuild and rerun:
   ```
   $ make veryclean && make
   $ cd ../../tests/c
   $ bash run.sh
   ```
3. Check `outfile` for lines such as:
   ```
   CPU time (sec)     = ...
   ```
4. Compare CPU time with the elapsed time from Step 1. Note that the resolution of the CPU timer is coarser; you may have to enlarge the array size (`nn` in `main.c`) or the loop counts to obtain a meaningful measurement.

### Step 3: Profile with gprof
1. Edit the `Makefile` again: disable the `-DUSE_*_TIMER` flags and enable the `-pg` line:
   ```makefile
   ## Use of gprof
   CFLAGS=-pg -g -Wall -O0 -std=gnu99
   ```
2. Rebuild:
   ```
   $ make veryclean && make
   ```
3. In `tests/c/run.sh`, uncomment the gprof lines:
   ```bash
   sleep 10s
   gprof $EXE > prof.out
   ```
4. Run the job script:
   ```
   $ cd ../../tests/c
   $ bash run.sh
   ```
5. The `gprof` result is summarized in `prof.out`. Examine the flat profile and the call graph to find the functions corresponding to the hotspot, and confirm that the result is consistent with the by-hand timer measurements from Steps 1 and 2.

## Questions to consider
1. Which function is the hotspot, and how do the call counts of `sub1`, `sub2`, and `sub3` explain it?
2. When do elapsed time and CPU time differ, and which one should you use for tuning?
3. What are the pros and cons of by-hand timers vs. `gprof`?

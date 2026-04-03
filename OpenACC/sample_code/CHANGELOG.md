# Change log
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).


## [1.0.0] - 2026-3-7
### Changed
- MIT license is adapted.
- The title of the lecture cource is changed "Introduction to accelerator" to "Introduction to GPU programming with OpenACC".
- The directory structure is rearranged.
- The contens in `stream-gpu` move to `stream/src/cuda` and `stream/tests/cuda`.

## [0.1.0] - 2025-5-13
### Modify
* Correct the source code so that it is the same as in Fortran, in lec01/simple.
* Set a restriction in lec3/stream/. Currently, vector length (acc) or num of threads per team (omptarget) must be a divisor of array size.
* Set a restriction in kokkos/stream/src/cpp.hierarchical. Currently, vector length must be a divisor of array size.
* Fix bugs in lec03/multi-gpu/jacobi-mpi.

## [0.0.3] - 2025-4-2
### Add
* Add deviceinfo sample with Fortran OpenACC 
* Add the stream codes with OpenACC, OpenMP Target, and Kokkos.
* Add MPI ping-pong with OpenMP Target

### Modify
* Marge deviceinfo sample with OpenMP Target into lec01/deviceinfo.
* Move saxpy sample with OpenMP Target to lec01/saxpy.
* Move array-reduction sample with OpenMP Target to lec01/array-reduction.
* Marge mpi-pingpong sample with OpenMP Target into lec03/multi-gpu/mpi-pingpong.
* Correct errors of acc directives in lec02/jacobi/src/c/acc2, lec03/jaboci-nvtx/src/c/acc2, lec03/multi-gpu/jacobi-mpi/src/c/acc2.dptr and acc2.update. 

## [0.0.2] - 2025-3-31
### Add
* Prepare deep copy, C++ clang, and fortran module. But not yet marge.

## [0.0.1] - 2025-3-28

## [0.0.0] - 2025-3-26

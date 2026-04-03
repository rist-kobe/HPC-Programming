# Change log
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2026-3-16
## Changed
- MIT license is adapted.
- The directory structure is rearranged.
- Dyhamical memory allocation is used in `jacobi` as the primary choise of memory allocation, rather than static memory.

## [0.2.3] - 2024-10-1
## Added
- Add the default settings for Makefile in `ompversion` and `masked` as `gnu`.
## Changed
- Modify Makefile in `ompversion`, `masked`, and `saxpy-gpu`.
- Correct misspelling.

## [0.2.2] - 2024-3-22
## Added
- Add a GPU-kernel example using OpenMP and OpenACC, `saxpy-gpu`.

## [0.2.1] - 2024-2-16
### Changed
- Re-number the name of the sample directories.
- Modify the evaluation in `schedule`, to take the absolute value of coordinate difference.

## [0.2.0] - 2024-1-31
## Added
- Add a remark on sample scripts for program execution in Mac M1.
- Add an instruction to use OpenMP in MacOS.
- Add Makefile for Mac M1 and Apple clang with LLVM OpenMP runtime in `hello`, `loop`, `schedule`, `jacobi`, `critical`, and `ompversion`.
- Add the dynamical-memory-allocation version of par of the codes in `jacobi`.
### Fixed
- Fix a writing mistake of OpenMP directive and the sample scripts in `critical`.

## [0.1.2] - 2024-1-25
## Added
- Add sample `ompversion` to check the version of OpenMP.
- Add sample `masked` for a program code with `masked` construct.

## [0.1.1] - 2024-1-24
### Added
- Add `critical` as an example.
- Add typical results of the sample codes to `readme.md`.
### Changed
- Rewrite README in the markdown format.
- Re-measure the perforamnce `loop`, `schedule`, and `jacobi`.
- Remove old terminology in `thdprivate`.
- Create `tests` directory to save a sample script for running the program.

## [0.1.0] - 2024-1-16
### Added
- Copyright to every source files.

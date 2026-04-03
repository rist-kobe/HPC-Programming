# Change log
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2026-03-06
### Changed
- MIT license is adapted.
- The directory structures are rearranged.

## [0.3.1] - 2025-09-18
### Changed
- Remove unnecessary reference about optimization of MD program in prof-ex.

## [0.3.0] - 2024-02-16
### Added
- Add a new sample `swp`.
### Changed
- Re-number the directory name, to put `swp` next to `simd_nsimple`.

## [0.2.2] - 2024-02-03
### Added
- Add timer with use of `CLOCK_MONOTONIC`. If setting `-DNOT_USE_REALTIME` in compiler options, one can use this version. Otherwise, one will use the timer with `CLOCK_REALTIME`.

## [0.2.1] - 2024-01-30
### Added
- Add Makefile for Mac M1 and Apple clang in `timer-res`, `alloc2d`, `thrashing`, `unroll`, `simd-add`, `simd-nsimple`, `polynomial`, and `io-format`.
- Add a remark on sample scripts for program execution in Mac M1.
### Changed
- Remove a wrong arch specific options for Mac  in `simd-add`.
### Fixed
- Fix mistakes in Makefile for GNU in `alloc2d/src/cpp`.

## [0.2.0] - 2024-01-26
### Changed
- Rewrite Makefile, `readme.md`, and sample scripts in `stripmining`.
- Rewrite Makefile and sample scripts in `matmatp`.
- Change sample `matmatp` to measure the timing results stably.
- Rewrite Makefile and `readme.md` in `thrashing`.
### Fixed
- Fix the description of GNU compiler version in `readme.md` of all the corresponding samples.
### Unfixed
- Find an issue that dumping a static array dose not work in `fortran_cbind` with Intel Fortran.

## [0.1.2] - 2024-01-23
### Changed
- Rewrite README in the markdown format for all of the samples.
- Rename `simd_add` into `simd-add`.
- Rename `simd_nsimple` into `simd-nsimple`.
- Rename `ET` into `polynomial`.
### Fixed
- Fix the setting of block sizes in `matmatp`. 
- Test the performance measurement in `matmatp` in AMD EPYC 7642/7643 and Intel Xeon 6126.
- Fix the kernels to check the occurrence of cache thrashing in `thrashing`.
- Fix mistakes and typos in `readme.md` in several samples. 

## [0.1.1] - 2024-01-18
### Added
- Add two kernels in `thrashing`.
### Changed
- Update the description of README in `timer-res`, `thrashing`, and `unroll`.
- Update the description of README in `simd_add`, `simd_nsimple`, `io-format`, and `endian`.
- Update the directory and file structures in `ET`.
- Rewrite README in the markdown format partially.
### Fixed
- Fix bugs in cpp in `alloc2d`.

## [0.1.0] - 2024-01-16
### Added
- Copyright to every source files.
- Prepare Makefile for LLVM (clang) for C/C++ samples in `timer`, `timer-res`, `matmatp`, `thrashing`, `unroll`, `simd_add`, `simd_nsimple`, `ET`, and `io-format`.

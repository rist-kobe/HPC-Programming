# Kokkos samples 
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: May 13rd, 2025

## Description
* This directory contains samples of using KOKKOS as backend of CUDA. 
* All the samples are experimental!
* You first need to check whether [Kokkos library](https://github.com/kokkos/kokkos) is available for your environment. If not, you first need to install Kokkos. Please see `00_install`.
* Currently, there is no document to run the samples. Kokkos provides good [tutorial materials](https://github.com/kokkos/kokkos-tutorials). Indeed, we borrowed some ideas from there. We suggest that you try Kokkos tutorial at first. 

## Tips
* To compile your source code with Kokkos, we recommend use of `nvcc_wrapper` given by Kokkos, which may be placed in `[kokkos_install_dir]/bin`.
* To execute the program with Kokkos, you may need to set `LD_LIBRARY_PATH` to read Kokkos library, such as `export LD_LIBRARY_PATH=[kokkos_install_dir]/lib64:${LD_LIBRARY_PATH}`.

## Script for executions
* Currently, only `daxypy` and `matvec` contain scripts for executing the program (`task.sh`).

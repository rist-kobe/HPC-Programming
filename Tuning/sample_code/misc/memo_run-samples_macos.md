# Memo for running samples in MacOS

* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: March 6th, 2026 

## Overview

Some Linux commands do not work or do not show useful information in MacOS (e.g., MacM1). You may find the alternatives. Please rewrite sample job scripts for executing the program.

## Examples

* Instead of `lscpu`, you can use `system_profiler SPHardwareDataType`.
* Instead of `getconf -a`, you can use `sysctl -a`.

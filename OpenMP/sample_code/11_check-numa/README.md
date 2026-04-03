# Check a NUMA setting in your machine
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: March 6th 2026

## Instruction
* This sample has no source code. We use Linux commands for checking the hardware information of CPU, `lscpu` and `numactl`. 
  * On MacOS, `numactl` may be nothing by default. You need to install it to run this sample.
    * Instead of `lscpu`, you can use `system_profiler SPHardwareDataType`.
* You can simply run the script in `test/task.sh`.

```shell
$ cd tests
$ bash task.sh 1> out.log 2> err.log
```


## Exercise
1. Check the outputs of `lscpu` and `numactl -H`. How many (sub-)nodes in a single node are there? 

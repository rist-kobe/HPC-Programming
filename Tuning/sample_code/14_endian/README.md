# Check endianness  
* Author:   Yukihiro Ota (yota@rist.or.jp)
* Last update: 18th Jan., 2024 

## Instruction: Compile
1. Source code is stored in `src/`. Choose either fortran or c.
2. Change directory
```
$ cd src/c                   # C
$ cd src/fortran/native      # Fortran, with system's native setting of endianness
$ cd src/fortran/swap        # Fortran, with the swapped version 
```
3. Make
```
$ make
```
The code is successfully compiled by GNU compiler (8.5.0) and Intel compiler (Intel oneAPI 2022.3.1).  
  * Remark: In fortran, you need to consider two patterns. 

## Instruction: Run and do analyses
1. Sample scripts are stored in `tests/`. Choose either fortran or c.       
2. Change directory
```
cd tests/c # On c
```
3. Run a job script, `run.sh`.
```
# One example
$ bash run.sh
# Another example
$ chmod 755 run.sh
$ ./run.sh
```
4. The results will be summarized in file, `logfile`

## Description
You may desire to save your results as a binary file, rather that a text (ASCII) file. In this case, you have to take care of endianness in your computer. Endianness means in what kinds of byte order data (integer and 
floating-point) is arranged. Two kinds of endianness are typically used.
  * little endian (examples: Intel x86, ARM)
  * big endian (examples: SPARC, IBM, Cray)  

In a Linux system, you can check a kind of endianness using `lscpu`. 

Let us consider an unsigned integer with 16 bit, for example, `0x1234`. This is a hexadecimal number. Notice that `34` consists of data with 1 byte (`= 4 bits *2 = 8 bits = 1 byte`). The byte order means the order of arranging upper byte (`12`) and lower bytes (`34``).   
``` 
Byte address:        0   1
little endian:      34  12
big endian:         12  34
```  

From a performance point of view the use of binary data in file I/O seems to be attractive, but we should recognize several disadvantages. See, e.g., Richard Gerber's presentation in High Performance Parallel I/O  (August 6, 2013) [http://www.nersc.gov/about/nersc-staff/center-leadership/richard-gerber](http://www.nersc.gov/about/nersc-staff/center-leadership/richard-gerber). 

## Exercise
1. Check the endianness of your system. 

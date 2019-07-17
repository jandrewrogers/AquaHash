# Introduction #

# How to #

### Compile the benchmark code ###

I have a hard time compile the code using GNU gcc, try clang if you do not want to see [misterious error messages](https://github.com/jandrewrogers/AquaHash/issues/1).

**Download the source code**

``` shell
git clone https://github.com/hungptit/AquaHash.git
```

**Compile all required libraries and binaries**

``` shell
cd AquaHash
./build.sh
```

**Plot the results**

``` shell
./plot_benchmark_results.py data/Darwin/string/data_ 4000 500
```

# Experiment setup #

## Test environments ##

### Linux ###

**CPU**
``` shell
Run on (88 X 2200.08 MHz CPU s)
CPU Caches:
  L1 Data 32K (x44)
  L1 Instruction 32K (x44)
  L2 Unified 256K (x44)
  L3 Unified 56320K (x2)
```

**Compiler**: clang-8.0.0

### MacOS ###

**CPU**

``` shell
Run on (8 X 2200 MHz CPU s)
CPU Caches:
  L1 Data 32K (x4)
  L1 Instruction 32K (x4)
  L2 Unified 262K (x4)
  L3 Unified 6291K (x1)
```

**Compiler**: clang-8.0.0

## Test data ##

### Hash functions ###

This benchmark will generate a random string then collect the performance benchmark results for each hash functions. The output data will be stored in JSON format.

### Hash table ###

This benchmark will use a hash table to construct a hash set from randomly generated strings.

# Results #

## Linux ##

**Short string benchmarks**

![Small string performance](small_string_linux.png)

**Large string benchmarks**

![Large string performance](large_string_linux.png)

## MacOS ##

**Short string benchmarks**

![Small string performance](small_string_macos.png)

**Large string benchmarks**

![Large string performance](large_string_macos.png)

<!-- ## Window Linux Subsystem ## -->

<!-- **Short string benchmarks** -->

<!-- ![Small string performance](small_string_subsystem.png)  -->

<!-- **Large string benchmarks** -->

<!-- ![Large string performance](large_string_subsystem.png)  -->

## aquahash vs md5sum vs xxhsum vs sha1sum commands ##

### Setup ###

### The small file benchmark i.e file size is less than 4Kb ###

``` shell
MacOS$ ./benchmark_commands -g small_file
Celero
Timer resolution: 0.001000 us
|     Group      |   Experiment    |   Prob. Space   |     Samples     |   Iterations    |    Baseline     |  us/Iteration   | Iterations/sec  |
|:--------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|
|small_file      | md5sum          |            Null |              10 |               1 |         1.00000 |     12123.00000 |           82.49 |
|small_file      | aquahash        |            Null |              10 |               1 |         0.96857 |     11742.00000 |           85.16 |
|small_file      | xxhash          |            Null |              10 |               1 |         0.94622 |     11471.00000 |           87.18 |
|small_file      | sha1sum         |            Null |              10 |               1 |         0.99794 |     12098.00000 |           82.66 |
|small_file      | sha224sum       |            Null |              10 |               1 |         1.29522 |     15702.00000 |           63.69 |
|small_file      | sha256sum       |            Null |              10 |               1 |         1.14130 |     13836.00000 |           72.28 |
|small_file      | sha384sum       |            Null |              10 |               1 |         0.99332 |     12042.00000 |           83.04 |
|small_file      | sha512sum       |            Null |              10 |               1 |         0.94845 |     11498.00000 |           86.97 |
Completed in 00:00:01.332131
```

### The mid file benchmark i.e file size is greater than 1Mb ###

``` shell
MacOS$ ./benchmark_commands -g mid_file
Celero
Timer resolution: 0.001000 us
|     Group      |   Experiment    |   Prob. Space   |     Samples     |   Iterations    |    Baseline     |  us/Iteration   | Iterations/sec  |
|:--------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|
|mid_file        | md5sum          |            Null |              10 |               1 |         1.00000 |     50164.00000 |           19.93 |
|mid_file        | aquahash        |            Null |              10 |               1 |         0.34814 |     17464.00000 |           57.26 |
|mid_file        | xxhash          |            Null |              10 |               1 |         0.35131 |     17623.00000 |           56.74 |
|mid_file        | sha1sum         |            Null |              10 |               1 |         0.96083 |     48199.00000 |           20.75 |
|mid_file        | sha224sum       |            Null |              10 |               1 |         1.98278 |     99464.00000 |           10.05 |
|mid_file        | sha256sum       |            Null |              10 |               1 |         1.96490 |     98567.00000 |           10.15 |
|mid_file        | sha384sum       |            Null |              10 |               1 |         1.23250 |     61827.00000 |           16.17 |
|mid_file        | sha512sum       |            Null |              10 |               1 |         1.24031 |     62219.00000 |           16.07 |
Completed in 00:00:05.335981
```

### The mid file benchmark i.e file size is greater than 200Mb ###

``` shell
MacOS$ ./benchmark_commands -g large_file
Celero
Timer resolution: 0.001000 us
|     Group      |   Experiment    |   Prob. Space   |     Samples     |   Iterations    |    Baseline     |  us/Iteration   | Iterations/sec  |
|:--------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|
|large_file      | md5sum          |            Null |              10 |               1 |         1.00000 |    610660.00000 |            1.64 |
|large_file      | aquahash        |            Null |              10 |               1 |         0.09552 |     58328.00000 |           17.14 |
|large_file      | xxhash          |            Null |              10 |               1 |         0.12302 |     75122.00000 |           13.31 |
|large_file      | sha1sum         |            Null |              10 |               1 |         0.94250 |    575545.00000 |            1.74 |
|large_file      | sha224sum       |            Null |              10 |               1 |         2.30607 |   1408223.00000 |            0.71 |
|large_file      | sha256sum       |            Null |              10 |               1 |         2.30164 |   1405520.00000 |            0.71 |
|large_file      | sha384sum       |            Null |              10 |               1 |         1.31498 |    803008.00000 |            1.25 |
|large_file      | sha512sum       |            Null |              10 |               1 |         1.31095 |    800542.00000 |            1.25 |
Completed in 00:01:04.776061
```

# Analysis #

## Usability ##

* farmhash is the worse in-term of usability. We cannot compile farmhash using provided configure script and have to use it as a header only library.
* aquahash is not compilable using GNU gcc and users must use clang to compile AquaHash code.
* std::hash does not take raw pointers and length. We need to use std::string when comparing the performance of all studied hash functions.

## General observations ##

* The performance of all hash functions is consistent in all platforms.
* AquaHash is the fastest hash function in all of our benchmarks.
* clhash starts to shine when the string size if greater than or equal to 96 bytes.
* farmhash also has a very good performance for both small and large string.
* std::hash has a reasonable performance.
* boost::hash function is very slow it is 50x slower than AquaHash and 20x slower than std::hash.
* aquahash command is faster than all available commands in mid and large file benchmark. Note that it is hard to draw any conclusion from the small file bencharks because the size of the file is less than 4K, which is the minimum size of a file block.

## Short string benchmark ##

* AquaHash is the best hash function i.e it is faster than all studied hash functions in all runs. This hash function can process about 6 bytes/cycle on average.
* boost::hash is the worse hash function and its run-time seem to be quadratic to the size of the string.
* clhash has very high overhead for small strings and it starts to catch up with farmhash when the string size is greater than or equal to 96 bytes.
* xxHash is not faster than std::hash for small strings.

## Large string benchmark ##

* AquaHash is still the fastest hash function. It can process 15 bytes/cycle.
* clhash is the second best hash function and its hashing speed is about 9 bytes/cycle.
* farmhash also has a very good performance. Our performance benchmark shows that farmhash can process 8-9 bytes/cycle.
* boost::hash is again the worse hash function. We should not use boost::hash if possible.

# TODO #
* Demonstrate that the collision rate of AquaHash is good enough for real applications.

# Introduction #

# How to #

### Compile the benchmark code ###

*GNU gcc will output a lot of warning messages, however, you can ignore these warnings.*

#### Download the source code ####

``` shell
git clone https://github.com/hungptit/AquaHash.git
```

#### Compile all required libraries and binaries ####

``` shell
cd AquaHash
./build.sh
```

### Run the benchmark ###

Assuming the build process goes smoothly, below are commands that we can use to collect the benchmark results

**Run individual benchmark for test hash functions**

``` shell
LEN=64 ./random_string
```

**Collect the benchmark results for a range**

``` shell
./run_benchmark.sh 128
```

### Plot the results ###

``` shell
./plot_benchmark_results.py data/Darwin/string/data_ 128 32
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

## Benchmarks ##

*Note: Benchmark results vary in different CPU architectures.*

### Hash functions ###

This benchmark will generate a random string then collect the performance benchmark results for each hash functions. The output data will be stored in JSON format.

### Checksum commands ###

This benchmark will compare the performance of auquahash command and popular checksum commands i.e xxhsum, md5sum, and sha1sum.

### Hash table (WIP) ###

This benchmark will measure the insertion, deletion, and lookup of std::unordered_map using different hash functions and test keys are randomly generated strings.

# Results #

## Linux ##

### Short string benchmarks ###

![Small string performance](benchmark/small_string_linux.png)

**Analysis**
* AquaHash is the fastest hash function for small keys with the average speed about 4 bytes/cycle.
* farmhash has a very good performance and its performance is jumped when the string size is divisible by 32.
* clhash is the second best hash function when the string size **is greater than or equal to 96 bytes**.
* boost::hash is the worse hash function and it is significantly slower than other hash functions.

### Large string benchmarks ###

![Large string performance](benchmark/large_string_linux.png)

**Analysis**
* The performance of most hash functions is linear to the size of test strings.
* AquaHash is still the fastest hash function and it can processes 10 bytes/cycle.
* clhash is the second best hash function.
* boost::hash and std::hash are slowest hash functions from our benchmark.

## MacOS ##

### Short string benchmarks ###

![Small string performance](benchmark/small_string_macos.png)

**Analysis**
* AquaHash is still the fastest hash function. It can processes 10bytes/cycle for strings that a longer than 64 bytes.
* clhash is the second best hash function for input strings that are greater than or equal to **96 bytes** or **string length is divisible by 8**. Note that clhash is slow for small strings because [the input strings must be significantly larger than a vector register (128 bits)](https://github.com/lemire/clhash).
* farmhash is the second best hash function for strings that are less than **96 bytes**.

### Large string benchmarks ###

![Large string performance](benchmark/large_string_macos.png)

**Analysis**
* AquaHash is the fastest hash function in this benchmark and it can processes about 15 bytes/cycle.
* clhash is still the second best hash function for large strings, however, it is still 60% slower than AquaHash from our benchmark.
* boost::hash is still the worse hash function in this benchmark. It is about 50x slower than AquaHash.

## AquaHash command benchmark ##

This benchmark will compare the runtime of aquahash command with other popular commands for computing checksum. Specifically, we will compare the performance of quahash with xxhsum64, md5sum, sha1sum, sha224sum, sha256sum, sha512sum.

### Small files i.e some KB ###

``` shell
./benchmark_commands -g small_file
Celero
Timer resolution: 0.001000 us
|     Group      |   Experiment    |   Prob. Space   |     Samples     |   Iterations    |    Baseline     |  us/Iteration   | Iterations/sec  |
|:--------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|
|small_file      | md5sum          |            Null |              10 |               1 |         1.00000 |      3494.00000 |          286.20 |
|small_file      | aquahash        |            Null |              10 |               1 |         0.71380 |      2494.00000 |          400.96 |
|small_file      | xxhash          |            Null |              10 |               1 |         0.88208 |      3082.00000 |          324.46 |
|small_file      | sha1sum         |            Null |              10 |               1 |         0.96852 |      3384.00000 |          295.51 |
|small_file      | sha224sum       |            Null |              10 |               1 |         1.05381 |      3682.00000 |          271.59 |
|small_file      | sha256sum       |            Null |              10 |               1 |         1.37464 |      4803.00000 |          208.20 |
|small_file      | sha384sum       |            Null |              10 |               1 |         1.08529 |      3792.00000 |          263.71 |
|small_file      | sha512sum       |            Null |              10 |               1 |         1.08872 |      3804.00000 |          262.88 |
Completed in 00:00:00.518678
```

### Medium files i.e 1MB or above ###

``` shell
./benchmark_commands -g mid_file
Celero
Timer resolution: 0.001000 us
|     Group      |   Experiment    |   Prob. Space   |     Samples     |   Iterations    |    Baseline     |  us/Iteration   | Iterations/sec  |
|:--------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|
|mid_file        | md5sum          |            Null |              10 |               1 |         1.00000 |     45724.00000 |           21.87 |
|mid_file        | aquahash        |            Null |              10 |               1 |         0.11112 |      5081.00000 |          196.81 |
|mid_file        | xxhash          |            Null |              10 |               1 |         0.14881 |      6804.00000 |          146.97 |
|mid_file        | sha1sum         |            Null |              10 |               1 |         1.19051 |     54435.00000 |           18.37 |
|mid_file        | sha224sum       |            Null |              10 |               1 |         2.59728 |    118758.00000 |            8.42 |
|mid_file        | sha256sum       |            Null |              10 |               1 |         2.59555 |    118679.00000 |            8.43 |
|mid_file        | sha384sum       |            Null |              10 |               1 |         1.73786 |     79462.00000 |           12.58 |
|mid_file        | sha512sum       |            Null |              10 |               1 |         1.73618 |     79385.00000 |           12.60 |
Completed in 00:00:05.775897
```

### Large files i.e 100MB or above ###

``` shell
./benchmark_commands -g large_file
Celero
Timer resolution: 0.001000 us
|     Group      |   Experiment    |   Prob. Space   |     Samples     |   Iterations    |    Baseline     |  us/Iteration   | Iterations/sec  |
|:--------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|
|large_file      | md5sum          |            Null |              10 |               1 |         1.00000 |   1458792.00000 |            0.69 |
|large_file      | aquahash        |            Null |              10 |               1 |         0.09018 |    131552.00000 |            7.60 |
|large_file      | xxhash          |            Null |              10 |               1 |         0.11189 |    163228.00000 |            6.13 |
|large_file      | sha1sum         |            Null |              10 |               1 |         1.19662 |   1745625.00000 |            0.57 |
|large_file      | sha224sum       |            Null |              10 |               1 |         2.66558 |   3888527.00000 |            0.26 |
|large_file      | sha256sum       |            Null |              10 |               1 |         2.66643 |   3889766.00000 |            0.26 |
|large_file      | sha384sum       |            Null |              10 |               1 |         1.77740 |   2592858.00000 |            0.39 |
|large_file      | sha512sum       |            Null |              10 |               1 |         1.77948 |   2595897.00000 |            0.39 |
Completed in 00:03:03.410126
```

### Analysis ###

* AquaHash is the fastest command in all of our benchmarks.
* xxhsum is the second best command and it is about 20% slower than AquaHash. Note that xxhsum is portable and AquaHash is not.
* The performance difference between the two best commands and other commands is 10x for medium and large files and it is the reason why xxHash is very popular for calculating the checksum in performance critical applications.

# Other observations #

* The performance of all hash functions is consistent in all platforms.

* farmhash is the worse in-term of usability. We cannot compile farmhash using provided configure script and have to use it as a header only library.

* AquaHash is not compilable using GNU gcc and **users must use clang to compile AquaHash code**.

* std::hash does not take raw pointers and length. We need to use std::string when comparing the performance of all studied hash functions.

* Both AquaHash and xxHash are good candidates for computing checksum. AquaHash may offer a higher hash quality than xxHash because it is 128bit hash function.

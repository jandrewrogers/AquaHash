* Introduction

* How to 

** Compile the benchmark code

I have a hard time compile the code using GNU gcc, try clang if you do not want to see [misterious error messages](https://github.com/jandrewrogers/AquaHash/issues/1).

**Download the source code**

**Compile all required libraries**

**Compile all benchmark binary**

**Collect the benchmark results**

**Plot the results**

* Experiment setup

** Environments

*** Linux

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

*** MacOS

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

** Test data

*** Hash functions
This benchmark will generate a random string then collect the performance benchmark results for each hash functions. The output data will be stored in JSON format. 

*** Hash table
This benchmark will use a hash table to construct a hash set from randomly generated strings.

* Benchmark results

* Linux

**Short string benchmarks**


**Long string benchmarks**

* MacOS

**Short string benchmarks**


**Long string benchmarks**


* Window Linux Subsystem

**Short string benchmarks**

**Long string benchmarks**


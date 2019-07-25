class: center, middle

# AquaHash: The fastest 128bit non-cryptogaphic hash function

---

# Why do we need a hash function #

* Hash table

* Checksum

---

# What is [SIMD](https://en.wikipedia.org/wiki/SIMD) #

> Single instruction, multiple data (SIMD) is a class of parallel computers in Flynn's taxonomy. It describes computers with multiple processing elements that perform the same operation on multiple data points simultaneously. Such machines exploit data level parallelism, but not concurrency: there are simultaneous (parallel) computations, but only a single process (instruction) at a given moment. SIMD is particularly applicable to common tasks such as adjusting the contrast in a digital image or adjusting the volume of digital audio. Most modern CPU designs include SIMD instructions to improve the performance of multimedia use. SIMD is not to be confused with SIMT, which utilizes threads.


---

# What does [_mm_aesenc_si128](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm_aesenc_si128&expand=233) do? #

* Synopsis

``` text
__m128i _mm_aesenc_si128 (__m128i a, __m128i RoundKey)
#include <wmmintrin.h>
Instruction: aesenc xmm, xmm
CPUID Flags: AES
```

* Description

Perform one round of an AES encryption flow on data (state) in a using the round key in RoundKey, and store the result in dst."

* Operation

``` text
state := a
a[127:0] := ShiftRows(a[127:0])
a[127:0] := SubBytes(a[127:0])
a[127:0] := MixColumns(a[127:0])
dst[127:0] := a[127:0] XOR RoundKey[127:0]
```

* Performance

``` text
Architecture	Latency	Throughput (CPI)
Skylake	        4	    1
Broadwell	    7	    1
Haswell	        7	    1
Ivy Bridge	    8	    1
```

---

# What is AquaHash?

* Originally created by [J. Andrew Rogers](https://github.com/jandrewrogers)

* My contributions:
  * Refactor and modernize the existing source code.
  * Add automated tests
  * Add an extensive benchmark
  * Add a quahash command and potentially several new commands.
  * Improving the existing algorithms.

---

# The small key algorithm

---

# The large key algorithm

* Use 4 **128bits lanes** to speed up the hash computation.

---

# How can we figure out that cutoff?

---

# How do we compare the performance of AquaHash with other popular hash functions?

--

``` text
String length: 128
2019-07-25 10:43:50
Running ./random_string
Run on (88 X 2200.08 MHz CPU s)
CPU Caches:
  L1 Data 32K (x44)
  L1 Instruction 32K (x44)
  L2 Unified 256K (x44)
  L3 Unified 56320K (x2)
Load Average: 10.62, 10.60, 9.40
------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations
------------------------------------------------------------------
std_hash_string               26.5 ns         26.5 ns     26429839
boost_hash_string              277 ns          277 ns      2537986
xxhash_string                 26.9 ns         26.9 ns     26042539
farmhash_string               27.1 ns         27.0 ns     25863855
clhash_string                 11.0 ns         11.0 ns     63898400
aquahash_string               9.42 ns         9.41 ns     74649215
aquahash_convert_string       9.39 ns         9.39 ns     74501785
```

---

# Why do we need to compute the checksum for our files?

* Find out duplicated files i.e **git status (SHA1)**

* [Validate the content of a file.](https://github.com/Cyan4973/xxHash/)

---

# Benchmark #

* Small files

``` text
./benchmark_commands -g small_file
Celero
Timer resolution: 0.001000 us
|     Group      |   Experiment    |   Prob. Space   |     Samples     |   Iterations    |    Baseline     |  us/Iteration   | Iterations/sec  |
|:--------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|
|small_file      | md5sum          |            Null |              10 |               1 |         1.00000 |      3341.00000 |          299.31 |
|small_file      | aquahash        |            Null |              10 |               1 |         0.70308 |      2349.00000 |          425.71 |
|small_file      | xxhash          |            Null |              10 |               1 |         0.86561 |      2892.00000 |          345.78 |
|small_file      | sha1sum         |            Null |              10 |               1 |         0.98204 |      3281.00000 |          304.79 |
|small_file      | sha224sum       |            Null |              10 |               1 |         0.94313 |      3151.00000 |          317.36 |
|small_file      | sha256sum       |            Null |              10 |               1 |         0.99880 |      3337.00000 |          299.67 |
|small_file      | sha384sum       |            Null |              10 |               1 |         0.96827 |      3235.00000 |          309.12 |
|small_file      | sha512sum       |            Null |              10 |               1 |         1.02484 |      3424.00000 |          292.06 |
Completed in 00:00:00.330681
```

* Medidum files

``` text
./benchmark_commands -g mid_file
Celero
Timer resolution: 0.001000 us
|     Group      |   Experiment    |   Prob. Space   |     Samples     |   Iterations    |    Baseline     |  us/Iteration   | Iterations/sec  |
|:--------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|
|mid_file        | md5sum          |            Null |              10 |               1 |         1.00000 |     45512.00000 |           21.97 |
|mid_file        | aquahash        |            Null |              10 |               1 |         0.11461 |      5216.00000 |          191.72 |
|mid_file        | xxhash          |            Null |              10 |               1 |         0.14649 |      6667.00000 |          149.99 |
|mid_file        | sha1sum         |            Null |              10 |               1 |         1.19360 |     54323.00000 |           18.41 |
|mid_file        | sha224sum       |            Null |              10 |               1 |         2.60832 |    118710.00000 |            8.42 |
|mid_file        | sha256sum       |            Null |              10 |               1 |         2.61689 |    119100.00000 |            8.40 |
|mid_file        | sha384sum       |            Null |              10 |               1 |         1.76969 |     80542.00000 |           12.42 |
|mid_file        | sha512sum       |            Null |              10 |               1 |         1.74517 |     79426.00000 |           12.59 |
Completed in 00:00:05.741133
```

* Large files

``` text
./benchmark_commands -g large_file
Celero
Timer resolution: 0.001000 us
|     Group      |   Experiment    |   Prob. Space   |     Samples     |   Iterations    |    Baseline     |  us/Iteration   | Iterations/sec  |
|:--------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|:---------------:|
|large_file      | md5sum          |            Null |              10 |               1 |         1.00000 |   1461029.00000 |            0.68 |
|large_file      | aquahash        |            Null |              10 |               1 |         0.09101 |    132964.00000 |            7.52 |
|large_file      | xxhash          |            Null |              10 |               1 |         0.11281 |    164814.00000 |            6.07 |
|large_file      | sha1sum         |            Null |              10 |               1 |         1.19754 |   1749636.00000 |            0.57 |
|large_file      | sha224sum       |            Null |              10 |               1 |         2.66217 |   3889513.00000 |            0.26 |
|large_file      | sha256sum       |            Null |              10 |               1 |         2.66456 |   3892997.00000 |            0.26 |
|large_file      | sha384sum       |            Null |              10 |               1 |         1.77215 |   2589158.00000 |            0.39 |
|large_file      | sha512sum       |            Null |              10 |               1 |         1.77694 |   2596159.00000 |            0.39 |
Completed in 00:03:02.454894
```

---

# Future work #

* Improve the performance of the small key algorithm.
* Add more utility commands for example a command which can find all duplicated files.
* Address feature requests from users.

---

# Conclusions #

* AquaHash can process upto 15 bytes/cycle and it is the fastest available hash function.

* aquahash command is also the fastest command for computing a hash code for a file in MacOS and Linux.

# Questions #

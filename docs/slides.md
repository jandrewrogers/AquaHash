class: center, middle

# AquaHash: The fastest 128bit hash function

# Why do we need a hash function #

* Hash table
* Checksum

# What is SIMD #

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

# What is AquaHash? 

* Originally created by [J. Andrew Rogers](https://github.com/jandrewrogers)

* My contributions:
  * Refactor and modernize the existing source code.
  * Add automated tests
  * Add an extensive benchmark 
  * Add a quahash command and potentially several new commands.
  * Improving the existing algorithms.

# Small file algorithm

# Big file algorithm

# How can we figure out that cutoff?

# How do we compare the performance of AquaHash with other popular hash functions?

# Why do we need to compute the checksum for our files?

* Find out duplicated files i.e **git status (SHA1)**

* [Validate the content of a file.](https://github.com/Cyan4973/xxHash/)

# Benchmark #

# Future work #

* Improve the performance of the small key algorithm.

* Add more utility commands for example a command which can find all duplicated files.

* Address feature requests from users.

# Conclusions #

* AquaHash can process upto 15 bytes/cycle and it is the fastest available hash function.

* aquahash command is also the fastest command for computing a hash code for a file in MacOS and Linux.

# Questions #


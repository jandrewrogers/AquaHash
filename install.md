* Steps

**Download the source code**

``` shell
git clone -r https://github.com/hungptit/AquaHash.git
```

**Compile all required libraries**

``` shell
cd AquaHash
./build.sh
```

**Compile and run all unit tests**

``` shell
cd AquaHash
cd unittests
cmake ./
make -j5
make test
```

**Run individual benchmarks**

``` shell
cd benchmark
LEN=64 ./random_string
```

Below is the sample output collected in MacOS

``` shell
MacOS$ LEN=64 ./random_string
String length: 64
Generated random string: 8U8naBbDGvn1KgTUxgf4N7djOoKLq3sFDu2jRl1iaA7a7AniReA3OsKeS1KiNL0Y
2019-07-15 13:53:53
Running ./random_string
Run on (8 X 2200 MHz CPU s)
CPU Caches:
  L1 Data 32K (x4)
  L1 Instruction 32K (x4)
  L2 Unified 262K (x4)
  L3 Unified 6291K (x1)
Load Average: 2.27, 3.69, 3.32
---------------------------------------------------------
Benchmark                  Time           CPU Iterations
---------------------------------------------------------
std_hash_string            7 ns          7 ns   97376402
boost_hash_string        125 ns        108 ns    6411254
xxhash_string             14 ns         14 ns   47943892
farmhash_string            8 ns          8 ns   81998899
clhash_string              7 ns          7 ns  105218855
aquahash_string            7 ns          7 ns  100521275
```

**Collect performance benchmark results for different string sizes**

``` shell
./run_benchmark.sh 128
```


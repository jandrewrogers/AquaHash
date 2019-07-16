
#include "boost/container_hash/hash.hpp"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <iostream>
#include <iterator>
#include <random>
#include <stdexcept>
#include <cstdlib>

#define XXH_INLINE_ALL
#include "xxhash.h"

#include "farmhash.h"
#include "farmhash.cc"
#include "aquahash.h"
#include "interface.h"
#include "utils.h"
#include "clhash.h"

std::string generate_random_string() {
    const char *var = std::getenv("LEN");
    if (var == nullptr) { throw std::runtime_error("LEN environment variable is required!"); }
    int len = std::stoi(var);
    std::cout << "String length: " << len << "\n";
    aquahash::CharGenerator gen;
    return gen(len);
}

const std::string test_string = generate_random_string();

// std::hash benchmark
void std_hash_string(benchmark::State &state) {
    std::hash<std::string> h;
    for (auto _ : state) { benchmark::DoNotOptimize(h(test_string)); }
}
BENCHMARK(std_hash_string);

// boost::hash
void boost_hash_string(benchmark::State &state) {
    boost::hash<std::string> h;
    for (auto _ : state) { benchmark::DoNotOptimize(h(test_string)); }
}
BENCHMARK(boost_hash_string);

// xxHash
void xxhash_string(benchmark::State &state) {
    unsigned long long const seed = 0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(XXH64(test_string.data(), test_string.size(), seed));
    }
}
// Register the function as a benchmark
BENCHMARK(xxhash_string);

// FarmHash
void farmhash_string(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(util::Hash64(test_string.data(), test_string.size()));
    }
}
BENCHMARK(farmhash_string);

// clhash benchmark
void clhash_string(benchmark::State &state) {
    util::CLHash clhash;
    for (auto _ : state) {
        benchmark::DoNotOptimize(clhash(test_string.data(), test_string.size()));
    }
}
BENCHMARK(clhash_string);

// AquaHash
const __m128i seed = _mm_setzero_si128();
void aquahash_string(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(AquaHash::Hash((const uint8_t*)test_string.data(), test_string.size(), seed));
    }
}
BENCHMARK(aquahash_string);

void aquahash_convert_string(benchmark::State &state) {
    aquahash::hash<std::string> h;
    for (auto _ : state) {
        benchmark::DoNotOptimize(h(test_string));
    }
}
BENCHMARK(aquahash_convert_string);

BENCHMARK_MAIN();

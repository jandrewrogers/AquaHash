#include <benchmark/benchmark.h>
#include "aquahash.h"
#include "interface.h"

std::vector<std::string> data;
std::unordered_set<std::string> std_hash;
std::unordered_set<std::string, aquahash::hash<std::string>, std::equal_to<std::string>> aquahash;

std:vector<std::string> create_test_data(const int N) {
    
}

void std_hash(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(h(test_string));
    }
}
BENCHMARK(std_hash);


BENCHMARK_MAIN();


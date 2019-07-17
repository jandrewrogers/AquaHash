#include "celero/Celero.h"
#include <cstdio>
constexpr int number_of_samples = 10;
constexpr int number_of_operations = 1;

namespace {

    int run(const std::string &cmd, const std::string &file) {
        std::string command = cmd + " " + file + " > /dev/null";
        return system(command.data());
    }
} // namespace

std::string mid_file = "3200.txt";
std::string small_file = "plot_benchmark_results.py";

CELERO_MAIN

BASELINE(small_file, md5sum, number_of_samples, number_of_operations) { ::run("md5sum", small_file); }

BENCHMARK(small_file, aquahash, number_of_samples, number_of_operations) {
    ::run("../commands/aquahash", small_file);
}

BENCHMARK(small_file, xxhash, number_of_samples, number_of_operations) {
    ::run("../3p/bin/xxhsum", small_file);
}

BENCHMARK(small_file, sha1sum, number_of_samples, number_of_operations) { ::run("sha1sum", small_file); }

BENCHMARK(small_file, sha224sum, number_of_samples, number_of_operations) {
    ::run("sha224sum", small_file);
}

BENCHMARK(small_file, sha256sum, number_of_samples, number_of_operations) {
    ::run("sha256sum", small_file);
}

BENCHMARK(small_file, sha384sum, number_of_samples, number_of_operations) {
    ::run("sha384sum", small_file);
}

BENCHMARK(small_file, sha512sum, number_of_samples, number_of_operations) {
    ::run("sha512sum", small_file);
}

// Mid file size benchmark
BASELINE(mid_file, md5sum, number_of_samples, number_of_operations) { ::run("md5sum", mid_file); }

BENCHMARK(mid_file, aquahash, number_of_samples, number_of_operations) {
    ::run("../commands/aquahash", mid_file);
}

BENCHMARK(mid_file, xxhash, number_of_samples, number_of_operations) {
    ::run("../3p/bin/xxhsum", mid_file);
}

BENCHMARK(mid_file, sha1sum, number_of_samples, number_of_operations) { ::run("sha1sum", mid_file); }

BENCHMARK(mid_file, sha224sum, number_of_samples, number_of_operations) { ::run("sha224sum", mid_file); }

BENCHMARK(mid_file, sha256sum, number_of_samples, number_of_operations) { ::run("sha256sum", mid_file); }

BENCHMARK(mid_file, sha384sum, number_of_samples, number_of_operations) { ::run("sha384sum", mid_file); }

BENCHMARK(mid_file, sha512sum, number_of_samples, number_of_operations) { ::run("sha512sum", mid_file); }

// Large file
const std::string large_file = ".database";

BASELINE(large_file, md5sum, number_of_samples, number_of_operations) { ::run("md5sum", large_file); }

BENCHMARK(large_file, aquahash, number_of_samples, number_of_operations) {
    ::run("../commands/aquahash", large_file);
}

BENCHMARK(large_file, xxhash, number_of_samples, number_of_operations) {
    ::run("../3p/bin/xxhsum", large_file);
}

BENCHMARK(large_file, sha1sum, number_of_samples, number_of_operations) { ::run("sha1sum", large_file); }

BENCHMARK(large_file, sha224sum, number_of_samples, number_of_operations) {
    ::run("sha224sum", large_file);
}

BENCHMARK(large_file, sha256sum, number_of_samples, number_of_operations) {
    ::run("sha256sum", large_file);
}

BENCHMARK(large_file, sha384sum, number_of_samples, number_of_operations) {
    ::run("sha384sum", large_file);
}

BENCHMARK(large_file, sha512sum, number_of_samples, number_of_operations) {
    ::run("sha512sum", large_file);
}


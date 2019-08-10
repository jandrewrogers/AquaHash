#!/usr/bin/env python3
import argparse
import matplotlib.pyplot as plt
import json
import numpy as np

def plot_results(prefix, N, YLIM):
    # Get the benchmark results for each hash functions.
    std_hash_results = np.zeros(N, dtype=float)
    boost_hash_results = np.zeros(N, dtype=float)
    xxhash_results = np.zeros(N, dtype=float)
    farmhash_results = np.zeros(N, dtype=float)
    clhash_results = np.zeros(N, dtype=float)
    aquahash_results = np.zeros(N, dtype=float)
    aquahash64_results = np.zeros(N, dtype=float)
    wyhash_results = np.zeros(N, dtype=float)
    results = np.zeros((N,9), dtype=float);
    x = np.zeros(N, dtype=float)
    for idx in range(1, N, 1):
        data_file = prefix + str(idx) + ".json"
        x[idx] = idx;
        col = 0;
        results[idx - 1][col] = idx;
        data = json.load(open(data_file))
        benchmark_results = data["benchmarks"]
        for info in benchmark_results:
            benchmark_name = info["name"]
            value = info["real_time"]
            if (benchmark_name == "std_hash_string"):
                std_hash_results[idx] = value
                col = 1
            elif (benchmark_name == "boost_hash_string"):
                boost_hash_results[idx] = value
                col = 2
            elif (benchmark_name == "xxhash_string"):
                xxhash_results[idx] = value
                col = 3
            elif (benchmark_name == "farmhash_string"):
                farmhash_results[idx] = value
                col = 4
            elif (benchmark_name == "clhash_string"):
                clhash_results[idx] = value
                col = 5
            elif (benchmark_name == "aquahash_string"):
                aquahash_results[idx] = value
                col = 6
            elif (benchmark_name == "aquahash64_string"):
                aquahash64_results[idx] = value
                col = 7
            elif (benchmark_name == "wyhash_string"):
                wyhash_results[idx] = value
                col = 8
            else:
                print(f"Unrecognized benchmark_name: '{benchmark_name}'")

            results[idx-1][col] = value;

    # Plot results
    lines = plt.plot(
        x, std_hash_results, "r^-",
        x, boost_hash_results, "bv-",
        x, xxhash_results, 'kp-',
        x, farmhash_results, 'y8-',
        x, clhash_results, 'cs-',
        x, aquahash_results, 'go-',
        x, aquahash64_results, 'gx-',
        x, wyhash_results, 'bo-',
    )
    plt.xlabel("String length (bytes)")
    plt.ylabel("Run time (ns)")
    plt.ylim(0, YLIM)
    plt.xlim(1, N)
    plt.grid()
    benchmarks = ('std::hash', 'boost::hash', 'xxHash', 'farmhash', 'clhash', 'aquahash', 'aquahash64', "wyhash")
    plt.legend(lines, benchmarks)
    plt.title("Hash function performance analysis using random strings");
    plt.show()

    # Write data to CSV files
    header_string = "len," + ','.join(benchmarks);
    np.savetxt("data.csv", results, fmt="%4f", delimiter=',',header=header_string);

# Parse input arguments
parser = argparse.ArgumentParser(description='Required input arguments.')
parser.add_argument('prefix', metavar='prefix', type=str,
                    help='Prefix of JSON data files')
parser.add_argument('maximum_length', metavar='maximum_length', type=int,
                    help='The maximum length of tested strings')
parser.add_argument('ylim', metavar='ylim', type=int,
                    help='The maximum length of tested strings')

args = parser.parse_args()
prefix = args.prefix
N = args.maximum_length
ylim = args.ylim
print("Prefix: ", prefix)
print("Maximum string length: ", N)
N = args.maximum_length

# Plot benchmark results
plot_results(prefix, N, ylim)

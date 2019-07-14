#!/bin/bash
N=4096
mkdir -p data

# Compile all benchmarks
cmake ./
make -j5

# Run all benchmark and plot the collected data.
for idx in `seq 1 1 $N`;
do
    LEN=$idx ./random_string --benchmark_format=json --benchmark_out="data/data_$idx.json"
done
./plot_benchmark_results.py data/data_ $N

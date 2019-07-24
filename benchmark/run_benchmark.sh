#!/bin/bash
N=$1
echo "Maximum string length: $N"

# Create output folder if neccesary.
mkdir -p data
osType=$(uname)

# Compile all benchmarks
cmake ./
make -j5

# Run all benchmark and plot the collected data.
data_dir="data/$osType/string/"
mkdir -p $data_dir
for idx in `seq 1 1 $N`;
do
    LEN=$idx ./random_string --benchmark_format=json --benchmark_out="$data_dir/data_$idx.json"
done


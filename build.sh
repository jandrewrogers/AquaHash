#!/bin/bash
root_dir="$PWD"
echo "root_dir = $root_dir"

# Update all 3p modules
git pull
git submodule init
git submodule update

# # Build all required libraries
pushd $root_dir/3p
sh build_all.sh
if [ ! -d "lib" ]; then
	mv lib64 lib
fi
popd

# Run all unittests
pushd $root_dir/unittests/
cmake ./
make -j5
make test
popd

# Build all benchmarks
pushd $root_dir/benchmark
cmake ./
make -j5
popd

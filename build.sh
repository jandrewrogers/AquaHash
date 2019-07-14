#!/bin/bash
root_dir=${PWD}

# Update all 3p modules
git pull
git submodules init
git submodule update

# Build all required libraries
pushd $root/3p
sh build_all.sh
popd

# Run all unittests
pushd unittests/
cmake ./
make -j5
make test
popd


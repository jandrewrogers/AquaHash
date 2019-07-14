#!/bin/bash
set -euo pipefail                               # Use Bash strict mode

printf "Build doctest\n";
./build_using_cmake.sh doctest "-DCMAKE_CXX_COMPILER=clang++" > /dev/null

printf "Build FMT\n"
./build_using_cmake.sh fmt "-DFMT_DOC=OFF -DFMT_TEST=OFF" > /dev/null

printf "Build benchmark\n"
./build_using_cmake.sh benchmark "-DBENCHMARK_ENABLE_GTEST_TESTS=OFF" > /dev/null

printf "Build google farmhash\n"
./build_using_configure.sh farmhash

printf "Build xxhash\n"
./build_using_make.sh xxhash


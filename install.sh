#!/bin/bash
set -euo pipefail
cmake ./ -DCMAKE_INSTALL_PREFIX=$1
make install

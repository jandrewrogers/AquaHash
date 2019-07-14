#!/bin/bash
set -euo pipefail				# Use Bash strict mode
source get_build_options.sh

PKGNAME=$1
EXTRA_MAKE_OPTIONS=${2:-""}
EXTRA_INSTALL_OPTIONS=${3:-""}

# Build given package
APKG_SRC=$SRC_DIR/$PKGNAME
APKG_BUILD_FOLDER=$APKG_SRC/$PKGNAME
APKG_PREFIX=$PREFIX

echo "Src folder: " $APKG_SRC
echo "Build folder: " $APKG_BUILD_FOLDER
echo "Prefix folder: " $APKG_PREFIX

# Build a given package
pushd $APKG_SRC
make clean
make $BUILD_OPTS $EXTRA_MAKE_OPTIONS
make PREFIX=$APKG_PREFIX install
popd

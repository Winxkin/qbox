#!/bin/bash
$build_type

# # Configure the build with CMake
cd ..
cmake -B build -DCPM_SOURCE_CACHE=$(pwd)/Packages -DLIBQEMU_TARGETS="aarch64;riscv64;riscv32" -DCMAKE_BUILD_TYPE="DEBUG"

# # Change directory to the build directory
cd build
make -j4


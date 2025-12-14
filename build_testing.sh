#!/bin/sh

# With Testing
cmake -S . -B build -DMATRIXLIBRARY_BUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j4

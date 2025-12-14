#!/bin/sh

# With Benchmarking
cmake -S . -B build -DMATRIXLIBRARY_BUILD_BENCHMARKS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build -j4

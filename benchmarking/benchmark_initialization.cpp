#include <benchmark/benchmark.h>
#include "matrix.h"
#include <armadillo>

// Benchmarking the Initialization of a Matrix with Zeros in Matrix Class
static void InitializationZeros_MatrixClass(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state) {
        Matrix A = Matrix::Zeros(n, n);
        benchmark::DoNotOptimize(A);
    }
    state.SetItemsProcessed(state.iterations() * n * n);
}

// Benchmarking the Initialization of a Matrix with Zeros in Armadillo
static void InitializationZeros_Armadillo(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state) {
        arma::mat A = arma::zeros(n, n);
        benchmark::DoNotOptimize(A.memptr());
    }
    state.SetItemsProcessed(state.iterations() * n * n);
}

// Benchmarking the Initialization of a Matrix with Random Values in Matrix Class
static void InitializationRandom_MatrixClass(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state) {
        Matrix A = Matrix::Random(n, n);
        benchmark::DoNotOptimize(A);
    }
    state.SetItemsProcessed(state.iterations() * n * n);
}

// Benchmarking the Initialization of a Matrix with Random Values in Armadillo
static void InitializationRandom_Armadillo(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state) {
        arma::mat A = arma::randu(n, n);
        benchmark::DoNotOptimize(A.memptr());
    }
    state.SetItemsProcessed(state.iterations() * n * n);
}

// Run benchmarking for different matrix sizes
BENCHMARK(InitializationZeros_MatrixClass)
  ->Arg(10)
  ->Arg(100)
  ->Arg(200)
  ->Arg(400);

BENCHMARK(InitializationZeros_Armadillo)
  ->Arg(10)
  ->Arg(100)
  ->Arg(200)
  ->Arg(400);

BENCHMARK(InitializationRandom_MatrixClass)
  ->Arg(10)
  ->Arg(100)
  ->Arg(200)
  ->Arg(400);

BENCHMARK(InitializationRandom_Armadillo)
  ->Arg(10)
  ->Arg(100)
  ->Arg(200)
  ->Arg(400);

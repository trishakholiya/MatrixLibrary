#include <benchmark/benchmark.h>
#include "matrix.h"
#include <armadillo>

static void BM_MatrixInitializationZeros_MatrixClass(benchmark::State& state) {
    int n = state.range(0);

    for (auto _ : state) {
        Matrix A = Matrix::Zeros(n, n);
        benchmark::DoNotOptimize(A);
    }

    state.SetItemsProcessed(state.iterations() * n * n);
}

static void BM_MatrixInitializationZeros_Armadillo(benchmark::State& state) {
    int n = state.range(0);

    for (auto _ : state) {
        arma::mat A = arma::zeros(n, n);
        benchmark::DoNotOptimize(A.memptr());
    }

    state.SetItemsProcessed(state.iterations() * n * n);
}

static void BM_MatrixInitializationRandom_MatrixClass(benchmark::State& state) {
    int n = state.range(0);

    for (auto _ : state) {
        Matrix A = Matrix::Random(n, n);
        benchmark::DoNotOptimize(A);
    }

    state.SetItemsProcessed(state.iterations() * n * n);
}

static void BM_MatrixInitializationRandom_Armadillo(benchmark::State& state) {
    int n = state.range(0);

    for (auto _ : state) {
        arma::mat A = arma::randu(n, n);
        benchmark::DoNotOptimize(A.memptr());
    }

    state.SetItemsProcessed(state.iterations() * n * n);
}

// benchmark for differenct sizes
BENCHMARK(BM_MatrixInitializationZeros_MatrixClass)
  ->Arg(10)
  ->Arg(100)
  ->Arg(200)
  ->Arg(400);

BENCHMARK(BM_MatrixInitializationZeros_Armadillo)
  ->Arg(10)
  ->Arg(100)
  ->Arg(200)
  ->Arg(400);

BENCHMARK(BM_MatrixInitializationRandom_MatrixClass)
  ->Arg(10)
  ->Arg(100)
  ->Arg(200)
  ->Arg(400);

BENCHMARK(BM_MatrixInitializationRandom_Armadillo)
  ->Arg(10)
  ->Arg(100)
  ->Arg(200)
  ->Arg(400);

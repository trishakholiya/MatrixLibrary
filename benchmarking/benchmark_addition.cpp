#include <benchmark/benchmark.h>
#include "matrix.h"
#include <armadillo>

static void BM_MatrixAddition_MatrixClass(benchmark::State& state) {
    int n = state.range(0);
    Matrix A = Matrix::Random(n, n);
    Matrix B = Matrix::Random(n, n);

    for (auto _ : state) {
        benchmark::DoNotOptimize(A + B);
    }

    state.SetItemsProcessed(state.iterations() * n * n);
}

static void BM_MatrixAddition_Armadillo(benchmark::State& state) {
    int n = state.range(0);
    arma::mat A = arma::randu<arma::mat>(n, n);
    arma::mat B = arma::randu<arma::mat>(n, n);

    for (auto _ : state) {
      arma::mat C = A + B;
      benchmark::DoNotOptimize(C.memptr()); // needs to be pointer to memory NOT arma object
    }

    state.SetItemsProcessed(state.iterations() * n * n);
}

// benchmark for differenct sizes
BENCHMARK(BM_MatrixAddition_MatrixClass)
  ->Arg(4)
  ->Arg(10)
  ->Arg(100)
  ->Arg(200)
  ->Arg(400)
  ->Arg(800);

BENCHMARK(BM_MatrixAddition_Armadillo)
  ->Arg(4)
  ->Arg(10)
  ->Arg(100)
  ->Arg(200)
  ->Arg(400)
  ->Arg(800);

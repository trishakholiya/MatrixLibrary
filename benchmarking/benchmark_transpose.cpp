#include <benchmark/benchmark.h>
#include "matrix.h"
#include <armadillo>

static void BM_MatrixTranspose_MatrixClass(benchmark::State& state) {
    int n = state.range(0);
    Matrix A = Matrix::Random(n, n);

    for (auto _ : state) {
      Matrix C = A.transpose();
      benchmark::DoNotOptimize(C);
    }

    state.SetItemsProcessed(state.iterations() * n * n);
}

static void BM_MatrixTranspose_Armadillo(benchmark::State& state) {
    int n = state.range(0);
    arma::mat A = arma::randu<arma::mat>(n, n);
  
    for (auto _ : state) {
      arma::mat C = A.t();
      benchmark::DoNotOptimize(C.memptr());
    }

    state.SetItemsProcessed(state.iterations() * n * n);
}

// benchmark for differenct sizes
BENCHMARK(BM_MatrixTranspose_MatrixClass)
  ->Arg(4)
  ->Arg(10)
  ->Arg(100)
  ->Arg(200)
  ->Arg(400)
  ->Arg(800);

BENCHMARK(BM_MatrixTranspose_Armadillo)
  ->Arg(4)
  ->Arg(10)
  ->Arg(100)
  ->Arg(200)
  ->Arg(400)
  ->Arg(800);

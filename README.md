# MatrixLibrary

Matrix class with linear algebra functionality

## Library Features


## Documentation

This project uses **Doxygen** to generate API documentation from source comments.

To generate documentation locally:

```bash
doxygen Doxyfile
open docs/html/index.html
```

# MatrixLibrary

## Overview
MatrixLibrary is a lightweight C++ linear algebra library developed to support core matrix operations and symmetric eigendecomposition. The primary goal of this project was to implement a minimal yet functional matrix library from scratch that could be used as a drop-in replacement for Armadillo in Homework 3.

The library supports basic matrix construction and arithmetic, matrix property checks, and eigendecomposition of real symmetric matrices using a Householder tridiagonalization followed by the QL algorithm. Accuracy and performance are validated through unit tests, numerical accuracy benchmarks, and runtime benchmarks.

---

## Features
- Matrix construction:
  - Zeros, ones, and identity matrices
  - Construction from flat data vectors
- Core matrix operations:
  - Element access
  - Addition and subtraction
  - Matrix–matrix multiplication
  - Scalar multiplication
  - Transpose
- Matrix properties:
  - Symmetry checks with configurable tolerance
- Linear algebra:
  - Eigenvalues and eigenvectors for real symmetric matrices
    - Householder tridiagonalization
    - QL algorithm with implicit shifts
- I/O utilities:
  - Saving matrices and vectors to HDF5 format
- Error handling:
  - Custom exception classes for invalid matrix operations

---

## Repository Structure
```
include/        # Public headers (Matrix API, exceptions, helpers)
src/            # MatrixLibrary implementation files
test/           # Unit tests and numerical accuracy validation
benchmarking/   # Runtime benchmarking using Google Benchmark
cmake/          # CMake configuration templates
docs/           # Generated documentation (Doxygen output)
.github/        # CI configuration (GitHub Actions)
```

---

## Build Instructions
MatrixLibrary uses CMake for configuration and builds.

### Requirements
- C++17 compatible compiler
- CMake ≥ 3.31
- Armadillo (used for validation and comparison)
- Eigen, BLAS, LAPACK
- HighFive / HDF5
- GoogleTest (optional, for unit tests)
- Google Benchmark (optional, for runtime benchmarks)

### Build Example
```bash
mkdir build
cd build
cmake .. -DMATRIXLIBRARY_BUILD_TESTS=ON -DMATRIXLIBRARY_BUILD_BENCHMARKS=ON
make
```

Unit tests and benchmarks can be enabled or disabled independently via CMake options.

---

## Usage
MatrixLibrary is intended primarily for educational and experimental use. It is designed to be compatible with existing Homework 3 code that previously relied on Armadillo.

Typical usage includes constructing matrices, performing arithmetic operations, and computing eigenvalues and eigenvectors using the `eigsym()` method for symmetric matrices.

---

## Testing and Validation
Unit tests are implemented using **GoogleTest** and are located in the `test/` directory.

The test suite validates:
- Matrix construction and accessors
- Arithmetic operations
- Transpose and symmetry checks
- Eigenvalue and eigenvector computation

Validation is performed by comparing MatrixLibrary results against Armadillo’s outputs, which serve as the reference implementation. Accuracy is quantified using maximum absolute error metrics, reconstruction error, and eigenvector orthogonality checks. Edge cases such as 1×1 matrices and diagonal matrices are explicitly tested.

---

## Accuracy Benchmarking
In addition to unit tests, numerical accuracy is evaluated using standalone accuracy benchmarks located in the `test/` directory.

These benchmarks:
- Generate random symmetric matrices across a range of sizes
- Compare eigensolver outputs against Armadillo
- Record eigenvalue error, reconstruction error, and orthogonality error

The resulting data is used to analyze how numerical error scales with matrix size.

---

## Performance Benchmarking
Runtime performance benchmarks are implemented using **Google Benchmark** and are located in the `benchmarking/` directory.

Benchmarks measure the performance of:
- Matrix initialization
- Element access
- Arithmetic operations
- Transpose
- Eigendecomposition

Performance is compared against Armadillo across multiple matrix sizes to provide context for the efficiency of the implementation.

---

## Design Decisions
- The eigensolver is restricted to real symmetric matrices to reduce implementation complexity and improve numerical stability.
- Householder tridiagonalization followed by the QL algorithm was chosen as a balance between efficiency and conceptual clarity.
- More general algorithms such as Hessenberg reduction and Jacobi iteration were not implemented due to increased complexity or computational cost.

---

## Limitations and Future Work
Current limitations include:
- Support limited to symmetric matrices for eigendecomposition
- No direct use of LAPACK routines

Potential future improvements include:
- Support for non-symmetric eigenvalue problems
- Integration with LAPACK for improved numerical stability and performance
- Additional matrix decompositions and linear algebra routines

---

## References
- W. H. Press et al., *Numerical Recipes: The Art of Scientific Computing*, 3rd ed.
- L. N. Trefethen and D. Bau III, *Numerical Linear Algebra*
- Sanderson et al., *Armadillo: a template-based C++ library for linear algebra*

# MatrixLibrary

## Overview
MatrixLibrary is a C++ library designed specifically to support computational chemistry workflows. This library is optimized for matrix operations associated with quantum chemical methods such as CNDO/2 and Extended Huckel Theory. The primary goal of this project was to implement a minimal yet functional matrix library from scratch that could be used as a drop-in replacement for Armadillo in Homework 3.

The library has many features including support for basic matrix construction, matrix arithmetic, and and eigendecomposition of real symmetric matrices using a Householder tridiagonalization followed by the QL algorithm. Accuracy and performance are validated through Google Tests, Google Benchmark and numerical accuracy benchmarks.

---

## Features
- Matrix construction:
  - Zeros, ones, and square identity matrices
  - Construction from vector
- Core matrix operations:
  - Element access
  - Addition
  - Subtraction
  - Matrix–matrix multiplication
  - Scalar multiplication
  - Transpose
- Matrix properties:
  - Symmetry checks
- Linear algebra:
  - Eigenvalues and eigenvectors for real symmetric matrices
    - Householder Tridiagonalization
    - QL algorithm
- Other Utilities:
  - Saving matrices and vectors to HDF5 format
- Error handling:
  - Custom exception classes for invalid matrix operations

---

## Repository Structure
```
include/        # Public headers
src/            # MatrixLibrary implementation files
test/           # Google Unit Tests and numerical accuracy validation
benchmarking/   # Google Benchmark Runtime Benchmarking
cmake/          # CMake configuration template
report/         # Final Project Report, Presentation, and Doxygen PDF
.github/        # CI configuration (GitHub Actions)
```

---

## Build Instructions
MatrixLibrary uses CMake for configuration and builds.

### Build Example
```bash
./build
```

To run benchmarking:
```bash
./build_benchmarking.sh
```

To run unit tests:
```bash
./build_testing.sh
```

## Usage
MatrixLibrary is intended primarily for educational and experimental use. It is designed to be compatible with existing Homework 3 code that previously relied on Armadillo.

To use this library in your local repo, add the following lines to the CMake File:

```cmake
include(FetchContent)

FetchContent_Declare(
    MatrixLibrary
    GIT_REPOSITORY https://github.com/trishakholiya/MatrixLibrary
    GIT_TAG main
)

FetchContent_MakeAvailable(MatrixLibrary)
```

Then, the matrix class can be used by adding `#include <matrix.h>`

---

## Testing and Validation
Unit tests are implemented using **GoogleTest** and are located in the `test/` directory.

The test suite validates:
- Matrix construction and accessors
- Addition, Subtraction and Multiplication Operators
- Transpose and symmetry checks
- Eigenvalue and eigenvector computation

Validation is performed by comparing MatrixLibrary results against Armadillo’s outputs. Accuracy is quantified using maximum absolute error metrics, reconstruction error, and eigenvector orthogonality checks. Edge cases such as 1×1 matrices and diagonal matrices are also tested.

---

## Accuracy Benchmarking
In addition to unit tests, numerical accuracy is evaluated using accuracy benchmarks located in the `test/` directory.

These benchmarks:
- Generate random symmetric matrices across a range of sizes
- Compare eigensolver outputs against Armadillo
- Record eigenvalue error, reconstruction error, and orthogonality error

This data is analyzed to see how numerical error scales with matrix size.

---

## Performance Benchmarking
Runtime performance benchmarks are implemented using **Google Benchmark** and are located in the `benchmarking/` directory.

Benchmarks measure the performance of:
- Matrix initialization
- Element access
- Multiplication, Subtraction, Addition
- Transposition
- Matrix decomposition into Eigenvalues and Eigenvectors

Performance is compared against Armadillo across matrix sizes.

---

## Future Work

Potential future improvements include:
- Support for non-symmetric eigenvalue problems
- Integration with LAPACK for improved numerical stability and performance
- Additional matrix decompositions and linear algebra routines

---

## References
- Goto, K., & van de Geijn, R. A. (2008). Anatomy of High-Performance Matrix Multiplication. ACM Transactions on Mathematical Software, 34(3), Article 12.
- J. Fritz, M. Mesiti, and J. P. Thiele, “A concise guide to good practices for automated testing and documentation of Research Software”, ECEASST, vol. 83, Feb. 2025.
- L. N. Trefethen and D. Bau III, Numerical Linear Algebra, SIAM, 1997, Lectures 10–11 and 27.
- McCalpin, J. D. Memory Bandwidth and Machine Balance in Current High Performance Computers. IEEE Technical Committee on Computer Architecture (TCCA) Newsletter, 1995.
- Sanderson et al., (2016), Armadillo: a template-based C++ library for linear algebra, Journal of Open Source Software, 1(2), 26, doi:10.21105/joss.00026
- Wilson G, Aruliah DA, Brown CT, Chue Hong NP, Davis M, Guy RT, Haddock SH, Huff KD, Mitchell IM, Plumbley MD, Waugh B, White EP, Wilson P. Best practices for scientific computing. PLoS Biol. 2014 Jan;12(1):e1001745. doi: 10.1371/journal.pbio.1001745. Epub 2014 Jan 7. PMID: 24415924; PMCID: PMC3886731.
- W. H. Press, S. A. Teukolsky, W. T. Vetterling, and B. P. Flannery, Numerical Recipes: The Art of Scientific Computing, 3rd ed., Cambridge University Press, 2007, Ch. 11.

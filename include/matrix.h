#pragma once

#include <iostream>
#include <vector>
#include <limits>
#include "custom_exception.hpp"
#include "helper_func.hpp" // numerical recipes helper functions
#include <cmath>
#include <iomanip>
#include <stdexcept>
#include <random>

/**
 * @mainpage MatrixLibrary
 *
 * @section intro_sec Introduction
 *
 * MatrixLibrary is a  matrix library with basic linear algebra
 * functionality including eigenvalue decompositions for symmetric matrices.
 *
 * @section features_sec Features
 * - Matrix arithmetic
 * - Householder tridiagonalization
 * - QL eigenvalue solver
 * - Symmetry Check
 * - Transpose
 * - HDF5 output
 */

/**
 * @brief Alias for vector of doubles, used for matrix storage
*/
typedef std::vector<double> vec;

// Forward declarations of global result types
struct TridiagonalResult;
struct EigsymResult;
struct QLEigenResult;

/**
 * @class Matrix
 * @brief Matrix class with basic linear algebra operations
 * 
 * The matrix class stores elements in contiguous memory to improve cache locality
 * This class supports multiplication, addition, subtraction, symmetry checks,
 * transposing matrix, saving to HDF5 file and eigenvalue decompositions
 * for symmetric matrices.
 */
class Matrix {
private:
  int num_rows;
  int num_cols;
  int size;
  vec matrix;

public:
  // === Constructors ===
  /**
   * @brief Construct a matrix of size (row * cols)
   */
  Matrix(int rows, int cols);
  Matrix();
  /**
   * @brief Construct a matrix of size (row * cols) with data
   */
  Matrix(const vec& values, int rows, int cols);
  Matrix(vec&& values, int rows, int cols);

  // === Factory Methods ===
  /**
   * @brief Fill matrix of specified size with 1's
   */
  static Matrix Ones(int rows, int cols);

  /**
   * @brief Fill matrix of specified size with 0's
   */
  static Matrix Zeros(int rows, int cols);

  /**
   * @brief Fill matrix of specific size with random doubles from [0.0, 1.0)
   */
  static Matrix Random(int rows, int cols);

  /**
   * @brief Create square identity matrix with dimensions n*n
   */
  static Matrix Identity(int n);

  // === Accessors ===
  int get_num_rows() const;
  int get_num_cols() const;
  int get_size() const;
  const vec& get_data() const;

  // === Operators ===
  /**
   * @brief Access a matrix element at (row, col)
   * 
   * @param x row index (0-based)
   * @param y column index (0-based)
   * @return reference to the matrix element
   * 
   * @throws OutofBounds exception if the indices are invalid
   */
  double& operator()(int x, int y);

  /**
   * @brief Const reference to the matrix element at (row, col)
   */
  const double& operator()(int x, int y) const;
  
  bool operator==(const Matrix& other) const;
  Matrix operator+(const Matrix& other) const;
  Matrix operator-(const Matrix& other) const;
  Matrix operator*(const Matrix& other) const;
  Matrix operator*(double s) const;

  // === Linear Algebra Functionality ===
  static Matrix diagmat(const vec& vector);
  static Matrix diagmat(const Matrix& mat);
  bool is_symmetric(double tol) const;
  Matrix transpose() const;
  TridiagonalResult householder_tridiagonalize(bool yesvecs = true) const;
  QLEigenResult QL(vec d, vec e) const;
  EigsymResult eigsym() const;

  // === Saving to HDF5 File ===
  static void save_hdf5(const Matrix& data, const std::string& filename, const std::string& dataset_name);
  static void save_hdf5(const vec& data, const std::string& filename, const std::string& dataset_name);

};

/**
 * @brief Result of Householder Tridiagonalization
 * 
 * This code is adapted from Numerical Recipes
 */
struct TridiagonalResult { 
    vec d; // diagonal elements
    vec e; // off-diagonal elements
    Matrix Q_house; // orthogonal transformation matrix
};

/**
 * @brief Result of QL Algorithm
 * 
 * This code is adapted from Numerical Recipes
 */
struct QLEigenResult {
    vec eigenvalues;
    Matrix Q_ql;
};

/**
 * @brief Final eigenvalues and eigenvectors of matrix
 * 
 * This code is adapted from Numerical Recipes
 */
struct EigsymResult {
    vec eigenvalues;
    Matrix eigenvectors;
};

// === Printing Functionality ===
std::ostream& operator<<(std::ostream& out, const Matrix & M);
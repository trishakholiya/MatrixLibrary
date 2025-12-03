#pragma once

#include <iostream>
#include <vector>
#include "custom_exception.cpp"

typedef std::vector<double> vec;
typedef std::vector<vec> mat;


class Matrix {
private:
  int num_rows;
  int num_cols;
  int size;
  mat matrix;
  vec data; // store flattened version of matrix for speed, MAYBE SHOULD CONSIDER ONLY STORING FLATTENED MATRIX

public:
  // initialize matrix of specific size with zeros
  Matrix(int rows, int cols) : num_rows(rows), num_cols(cols), size(0), matrix(rows, vec(cols, 0.0)) {}

  // empty constructor
  Matrix() : num_rows(0), num_cols(0), size(0) {}


  // constructor from FLAT vector
  Matrix(const vec& values, int rows, int cols)
    : num_rows(rows), num_cols(cols), size(num_rows*num_cols)
  {
    // make sure this matrix can be constructed
    if (values.size() != rows * cols) {
        throw InvalidMatrixSize("Flat vector size does not match requested matrix dimensions");
    }

    matrix.resize(rows, vec(cols));
    int idx = 0; // for indexing into vector
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = values[idx];
            idx++;
        }
    }
  }

  // equal to a matrix operator
  // overload print << operator


  // overload addition operator
  inline Matrix operator+(const Matrix& other) const {
    // check if matrices are able to be added
    if (this->num_cols != other.num_cols || this->num_rows != other.num_rows) {
      throw InvalidMatrixSize("Matrix sizes must be equivalent for summation");
    }

    Matrix result(num_rows, num_cols);

    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            result.matrix[i][j] = matrix[i][j] + other.matrix[i][j];
        }
    }
      
    return result;
  }

  void print() const {
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            std::cout << matrix[i][j] << "  "; // need to figure out spacing between numbers
        }
        std::cout << "\n"; // line between rows
    }
    std::cout << "\n";
  }

  double L2_norm() {
    // need to implement this for benchmarking
    return 0.0;
  }

  // transpose matrix
  Matrix transpose() const {
    Matrix result(num_cols, num_rows);

    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            result.matrix[j][i] = matrix[i][j];
        }
    }

    return result;
  }

  /*
  std::pair<vec, mat> compute_eigen_decomposition() {
    // returns eigenvalues and eigenvectors at the same time 
    // so you don't have to recompute 3 times

    // implement
  }
  vec get_eigenvalues() const {
    // implement getter
  }

  Matrix get_eigenvectors() const {
    // implement getter
  }

  // diagonalize matrix
  Matrix diagonalize() const {
    Matrix result(num_rows, num_cols);
    // tridiagonal 
    // A = PDP^-1
    // compute P (eigenvectors matrix of A) and check if it has full rank
    // if full rank then it's diagonalizable, if not throw error (need to include a tolerance in the rank check)

    // implement

    return result;
  }
  */
};

#include <iostream>
#include <vector>
#include <stdexcept>

typedef std::vector<double> vec;
typedef std::vector<vec> mat;

class Matrix {
private:
  int num_rows;
  int num_cols;
  mat matrix;

public:
  // initialize matrix of specific size with zeros
  Matrix(int rows, int cols) : num_rows(rows), num_cols(cols), matrix(rows, vec(cols, 0.0)) {}

  // empty constructor
  Matrix() : num_rows(0), num_cols(0) {}

  // overload addition operator
  Matrix operator+(const Matrix& other) const {
      // check if matrices are able to be added
      if (this->num_cols != other.num_cols || this->num_rows != other.num_rows) {
        throw std::runtime_error("SHOULD PROBABLY DEFINE CUSTOM EXCEPTION");
      }

      Matrix result(num_rows, num_cols);

      for (int i = 0; i < num_rows; i++) {
          for (int j = 0; j < num_cols; j++) {
              result.matrix[i][j] = matrix[i][j] + other.matrix[i][j];
          }
      }

      return result;
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

  Matrix get_eigenvalues() const {
    // implement
  }

  Matrix get_eigenvectors() const {
    // implement
  }

  // diagonalize matrix
  Matrix diagonalize() const {
    Matrix result(num_rows, num_cols);
    // A = PDP^-1
    // compute P (eigenvectors matrix of A) and check if it has full rank
    // if full rank then it's diagonalizable, if not throw error (need to include a tolerance in the rank check)

    // implement

    return result;
  }
};
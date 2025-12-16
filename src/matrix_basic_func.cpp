#include "matrix.h"
#include <algorithm>

// Constructs an unitialized matrix of given dimension
Matrix::Matrix(int rows, int cols) : num_rows(rows), num_cols(cols), size(rows*cols), matrix(size) {}

// Default Constructor, creates a 0x0 matrix
Matrix::Matrix() : num_rows(0), num_cols(0), size(0) {}

// Construct a matrix from an existing flat vector (l-value reference).
Matrix::Matrix(const vec& values, int rows, int cols)
  : num_rows(rows), 
    num_cols(cols), 
    size(rows*cols)
{
  // Validate Dimensions
  if (values.size() != rows * cols) {
      throw InvalidMatrixSize("Flat vector size does not match requested matrix dimensions");
  }
  matrix = values;
}

// Construct a matrix from an existing flat vector (r-value reference).
Matrix::Matrix(vec&& values, int rows, int cols)
  : num_rows(rows), 
    num_cols(cols), 
    size(rows*cols),
    matrix(std::move(values)) {
    // Validate Dimensions
    if (matrix.size() != rows * cols) {
      throw InvalidMatrixSize("Flat vector size does not match requested matrix dimensions");
    }
  }

// -------------------------------------------------------------------
// Factory Methods to Fill Matrix Values
// -------------------------------------------------------------------

Matrix Matrix::Ones(int rows, int cols) {
  Matrix M(rows, cols);
  std::fill(M.matrix.begin(), M.matrix.end(), 1.0);
  return M;
}

Matrix Matrix::Zeros(int rows, int cols) {
  Matrix M(rows, cols);
  std::fill(M.matrix.begin(), M.matrix.end(), 0.0);
  return M;
}

Matrix Matrix::Random(int rows, int cols) {
  Matrix M(rows, cols);
  
  std::random_device rd;
  std::mt19937 generator(rd());

  std::uniform_real_distribution<double> distribution(0.0, 1.0);
  std::generate(M.matrix.begin(), M.matrix.end(), [&]() { return distribution(generator); });
  return M;
}

Matrix Matrix::Identity(int n) {
  Matrix M(n, n);
  std::fill(M.matrix.begin(), M.matrix.end(), 0.0);
  for (int i = 0; i < n; i++)
      M(i, i) = 1.0;
  return M;
}

// -------------------------------------------------------------------
// Accessors (const getters)
// -------------------------------------------------------------------
int Matrix::get_num_rows() const
{
  return this->num_rows;
}

int Matrix::get_num_cols() const
{
  return this->num_cols;
}

int Matrix::get_size() const
{
  return this->size;
}

const vec& Matrix::get_data() const
{
  return this->matrix;
}
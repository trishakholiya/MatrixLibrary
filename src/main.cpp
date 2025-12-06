//#include <armadillo>
//#include "benchmark.cpp"
#include "Matrix.hpp"
#include <vector>

void print_vec(const std::vector<double>& v, const std::string& name) {
    std::cout << name << ": ";
    for (double x : v) std::cout << x << " ";
    std::cout << "\n";
}

int main(int argc, char* argv[]) {

  // initialize zero matrix
  Matrix mat1(2, 2);
  mat1.print();

  // initialize matrix with values
  Matrix mat2({1, 2, 3, 4}, 2, 2);
  mat2.print();

  // initialize matrix with values
  Matrix mat3({5, 6, 7, 8}, 2, 2);
  mat3.print();

  // add matrixes
  Matrix mat4 = mat2 + mat3;
  mat4.print();

  /*
  // benchmark the time
  auto matrix_sum_func = [&]() {
    Matrix temp = mat2 + mat3;
  };

  
  // initialize matrixes with same values in armadillo
  arma::mat A = { {1, 2},
                  {3, 4} };

  arma::mat B = { {5, 6},
                  {7, 8} };

  auto arma_sum_func = [&]() {
    arma::mat temp = A + B;
  };

  double matrix_time = benchmark_time(matrix_sum_func);
  double arma_time = benchmark_time(arma_sum_func);

  std::cout << "Matrix Sum Time: " << matrix_time << '\n';
  std::cout << "Armadillo Sum Time: " << arma_time << '\n';
  */

  // test tridiagonalize function
  TridiagonalResult tri1 = mat1.householder_tridiagonalize(true);

  print_vec(tri1.d, "d1");
  print_vec(tri1.e, "e1");

  std::cout << "Q_house1:\n";
  tri1.Q_house.print();


  TridiagonalResult tri2 = mat2.householder_tridiagonalize(true);

  print_vec(tri2.d, "d2");
  print_vec(tri2.e, "e2");

  std::cout << "Q_house2:\n";
  tri2.Q_house.print();


  TridiagonalResult tri3 = mat3.householder_tridiagonalize(true);

  print_vec(tri3.d, "d3");
  print_vec(tri3.e, "e3");

  std::cout << "Q_house3:\n";
  tri3.Q_house.print();


  return 0;
}
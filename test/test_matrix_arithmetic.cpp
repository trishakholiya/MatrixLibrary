// test/test_matrix_arithmetic.cpp
#include <gtest/gtest.h>
#include <tuple> // for std::tuple in parameterized mul test
#include "matrix.h"
#include "test_helpers.hpp"

// file includes tests for add, subract, multiply, multiply by scalar

// creates 2 random 3 x 3 matrices using Matrix lib, adds them.
// converts same matrices to armadillo and adds, then compares the results.
TEST(MatrixArithmetic, AddMatchesArmadillo) {
    Matrix A = Matrix::Random(3,3);
    Matrix B = Matrix::Random(3,3);

    Matrix C_my = A + B;

    arma::mat A_ref = to_arma(A);
    arma::mat B_ref = to_arma(B);
    arma::mat C_ref = A_ref + B_ref;

    EXPECT_TRUE(mats_close(C_my, C_ref));
}

// same but for multiplication of 2 matrices
TEST(MatrixArithmetic, MulMatchesArmadillo) {
    Matrix A = Matrix::Random(2,3);
    Matrix B = Matrix::Random(3,4);

    Matrix C_my = A * B;

    arma::mat A_ref = to_arma(A);
    arma::mat B_ref = to_arma(B);
    arma::mat C_ref = A_ref * B_ref;

    EXPECT_TRUE(mats_close(C_my, C_ref));
}

// same but for multiplying matrix by a scalar
TEST(MatrixArithmetic, ScalarMulMatchesArmadillo) {
    Matrix A = Matrix::Random(3,3);
    double s = -2.5;

    Matrix C_my = A * s;
    arma::mat C_ref = to_arma(A) * s;

    EXPECT_TRUE(mats_close(C_my, C_ref));
}

// ##### PARAMETERIZED TESTS FOR DIFFERENT SIZES #####

// fixture (test class) that holds the int parameter (the matrix size n)
class MatrixAddSizeTest : public ::testing::TestWithParam<int> {};

// runs once for each n instantiated below
TEST_P(MatrixAddSizeTest, AddMatchesArmadilloForSize) {
    int n = GetParam();  // current size being tested

    Matrix A = Matrix::Random(n, n);
    Matrix B = Matrix::Random(n, n);

    Matrix C_my = A + B;

    arma::mat A_ref = to_arma(A);
    arma::mat B_ref = to_arma(B);
    arma::mat C_ref = A_ref + B_ref;

    double max_err = max_abs_error(C_my, C_ref);

    std::cout << "[Add] n = " << n << "  max_abs_error = " << max_err << "\n";

    EXPECT_TRUE(mats_close(C_my, C_ref))
        << "Addition failed for size " << n << "x" << n;
}

// tell GoogleTest which sizes to run this test for
// i.e. run MatrixAddSizeTest.AddMatchesArmadilloForSize() once for each n in {1, 2, 3, 5, 10}
INSTANTIATE_TEST_SUITE_P(
    AddSizes,              // prefix printed in test output
    MatrixAddSizeTest,     // test class
    ::testing::Values(1, 2, 3, 5, 10) // sizes to test
);

// type used to describe matrix shapes (rowsA, colsA, rowsB, colsB)
using ShapeParam = std::tuple<int, int, int, int>;

// test class that receives shapeparam for each run
class MatrixMulShapeTest : public ::testing::TestWithParam<ShapeParam> {};

TEST_P(MatrixMulShapeTest, MulMatchesArmadilloForShape) {
    auto [r1, c1, r2, c2] = GetParam();  // unpack tuple (rows_A, cols_A, rows_B, cols_B)

    // sanity check that inner dimensions match
    ASSERT_EQ(c1, r2) << "Invalid shape in test parameters";

    Matrix A = Matrix::Random(r1, c1);
    Matrix B = Matrix::Random(r2, c2);

    Matrix C_my = A * B;

    arma::mat A_ref = to_arma(A);
    arma::mat B_ref = to_arma(B);
    arma::mat C_ref = A_ref * B_ref;

    // compute max per-element absolute error
    double max_err = max_abs_error(C_my, C_ref);

    // print it to see accuracy by shape
    std::cout << "[Mul] "
              << r1 << "x" << c1 << " * " << r2 << "x" << c2
              << "  max_abs_error = " << max_err << "\n";

    EXPECT_TRUE(mats_close(C_my, C_ref))
        << "Multiply failed for shapes "
        << r1 << "x" << c1 << " * " << r2 << "x" << c2
        << "  (max_abs_error = " << max_err << ")";
}

// list of shapes to test
INSTANTIATE_TEST_SUITE_P(
    MulShapes,
    MatrixMulShapeTest,
    ::testing::Values(
        ShapeParam{2, 2, 2, 2}, // small square * square
        ShapeParam{2, 3, 3, 4}, // wide * tall
        ShapeParam{5, 1, 1, 7}, // column * row
        ShapeParam{1, 5, 5, 1}, // row * column
        ShapeParam{3, 3, 3, 3} // another square
    )
);
// test/test_matrix_arithmetic.cpp
#include <gtest/gtest.h>
#include <tuple> // for std::tuple in parameterized mul test
#include "matrix.h"
#include "test_helpers.hpp"

// file includes tests for add, subract, multiply, multiply by scalar

static const char* BASIC_CSV = "basic_accuracy.csv";

// ##### PARAMETERIZED TESTS FOR DIFFERENT SIZES #####

// fixture (test class) that holds the int parameter (the matrix size n)
class MatrixSizeTest : public ::testing::TestWithParam<int> {};

// creates 2 random n x n matrices using Matrix lib, adds them.
// converts same matrices to armadillo and adds, then compares the results.
TEST_P(MatrixSizeTest, AddMatchesArmadilloForSize) {
    int n = GetParam();  // current size being tested

    Matrix A = Matrix::Random(n, n);
    Matrix B = Matrix::Random(n, n);

    Matrix C_mat_lib = A + B;

    arma::mat A_ref = to_arma(A);
    arma::mat B_ref = to_arma(B);
    arma::mat C_ref = A_ref + B_ref;

    double max_err = max_abs_error(C_mat_lib, C_ref);

    append_csv(BASIC_CSV,
               "Arithmetic",
               "add",
               n, n,
               n, n,
               max_err);

    EXPECT_TRUE(mats_close(C_mat_lib, C_ref))
        << "Addition failed for size " << n << "x" << n;
}

// same structure but for subtraction
TEST_P(MatrixSizeTest, SubMatchesArmadilloForSize) {
    int n = GetParam();  // current size being tested

    Matrix A = Matrix::Random(n, n);
    Matrix B = Matrix::Random(n, n);

    Matrix C_mat_lib = A - B;

    arma::mat A_ref = to_arma(A);
    arma::mat B_ref = to_arma(B);
    arma::mat C_ref = A_ref - B_ref;

    double max_err = max_abs_error(C_mat_lib, C_ref);

    append_csv(BASIC_CSV,
               "Arithmetic",
               "sub",
               n, n,
               n, n,
               max_err);

    EXPECT_TRUE(mats_close(C_mat_lib, C_ref))
        << "Subtraction failed for size " << n << "x" << n;
}

// same structure but for multiplying matrix by a scalar
TEST_P(MatrixSizeTest, ScalarMulMatchesArmadilloForSize) {
    int n = GetParam();  // current size being tested
    double s = -2.5;

    Matrix A = Matrix::Random(n, n);

    Matrix C_mat_lib = A * s;
    arma::mat C_ref = to_arma(A) * s;

    double max_err = max_abs_error(C_mat_lib, C_ref);

    append_csv(BASIC_CSV,
               "Arithmetic",
               "scalar_mul",
               n, n,
               0, 0,
               max_err);

    EXPECT_TRUE(mats_close(C_mat_lib, C_ref))
        << "Scalar multiply failed for size " << n << "x" << n;
}

// tell GoogleTest which sizes to run these tests for
// i.e. run MatrixSizeTest.<test>() once for each n in {1, 2, 3, 5, 10}
INSTANTIATE_TEST_SUITE_P(
    Sizes,                 // prefix printed in test output
    MatrixSizeTest,        // test class
    ::testing::Values(1, 2, 3, 5, 10) // sizes to test
);

// type used to describe matrix shapes (rowsA, colsA, rowsB, colsB)
using ShapeParam = std::tuple<int, int, int, int>;

// test class that receives shapeparam for each run
class MatrixMulShapeTest : public ::testing::TestWithParam<ShapeParam> {};

// same but for multiplication of 2 matrices
TEST_P(MatrixMulShapeTest, MulMatchesArmadilloForShape) {
    auto [r1, c1, r2, c2] = GetParam();  // unpack tuple (rows_A, cols_A, rows_B, cols_B)

    // sanity check that inner dimensions match
    ASSERT_EQ(c1, r2) << "Invalid shape in test parameters";

    Matrix A = Matrix::Random(r1, c1);
    Matrix B = Matrix::Random(r2, c2);

    Matrix C_mat_lib = A * B;

    arma::mat A_ref = to_arma(A);
    arma::mat B_ref = to_arma(B);
    arma::mat C_ref = A_ref * B_ref;

    // compute max per-element absolute error
    double max_err = max_abs_error(C_mat_lib, C_ref);

    append_csv(BASIC_CSV,
               "Arithmetic",
               "mul",
               r1, c1,
               r2, c2,
               max_err);

    EXPECT_TRUE(mats_close(C_mat_lib, C_ref))
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
        ShapeParam{3, 3, 3, 3}  // another square
    )
);

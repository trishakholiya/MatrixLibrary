#include <gtest/gtest.h>
#include "matrix.h"
#include "test_helpers.hpp"

// this file includes tests for constructors, 
// accessors, ==, transpose, is_symmetric

// sizes and initialization (no Armadillo comparison for this part)

TEST(MatrixBasics, SizesAndInitialization) {
    Matrix A(3, 4);

    EXPECT_EQ(A.get_num_rows(), 3);
    EXPECT_EQ(A.get_num_cols(), 4);
    EXPECT_EQ(A.get_size(), 12);

    // default constructor, initialize zeros
    for (int i = 0; i < A.get_num_rows(); ++i) {
        for (int j = 0; j < A.get_num_cols(); ++j) {
            EXPECT_DOUBLE_EQ(A(i, j), 0.0);
        }
    }
}

// construct from std::vector and compare to Armadillo

TEST(MatrixBasics, FromVectorConstructorMatchesArmadillo) {
    vec values = {1.0, 2.0, 3.0,
                  4.0, 5.0, 6.0};
    Matrix M(values, 2, 3);

    arma::mat A_ref(2, 3);
    A_ref << 1.0 << 2.0 << 3.0 << arma::endr
          << 4.0 << 5.0 << 6.0 << arma::endr;

    double max_err = max_abs_error(M, A_ref);
    std::cout << "[Basics] FromVector max_abs_error = " << max_err << "\n";

    EXPECT_TRUE(mats_close(M, A_ref));
}

// testing helper methods that build specific matrices (zeros, ones, identity)

// testing identity marix builds correctly
TEST(MatrixBasics, IdentityCorrect) {
    int n = 5;
    Matrix I = Matrix::Identity(n);

    EXPECT_EQ(I.get_num_rows(), n);
    EXPECT_EQ(I.get_num_cols(), n);

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            EXPECT_DOUBLE_EQ(I(i,j), (i == j ? 1.0 : 0.0));
}

// testing zero matrix builds correctly
TEST(MatrixBasics, ZerosCorrect) {
    int n = 4, m = 6;
    Matrix Z = Matrix::Zeros(n, m);

    EXPECT_EQ(Z.get_num_rows(), n);
    EXPECT_EQ(Z.get_num_cols(), m);

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            EXPECT_DOUBLE_EQ(Z(i,j), 0.0);
}

// testing matrix of ones builds correctly
TEST(MatrixBasics, OnesCorrect) {
    int n = 3, m = 2;
    Matrix O = Matrix::Ones(n, m);

    EXPECT_EQ(O.get_num_rows(), n);
    EXPECT_EQ(O.get_num_cols(), m);

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            EXPECT_DOUBLE_EQ(O(i,j), 1.0);
}

// element access and operator== (no Armadillo comp)

TEST(MatrixBasics, ElementAccessAndEquality) {
    Matrix A(2, 2);
    A(0,0) = 1.5;
    A(0,1) = -2.0;
    A(1,0) = 3.0;
    A(1,1) = 4.5;

    EXPECT_DOUBLE_EQ(A(0,0), 1.5);
    EXPECT_DOUBLE_EQ(A(0,1), -2.0);
    EXPECT_DOUBLE_EQ(A(1,0), 3.0);
    EXPECT_DOUBLE_EQ(A(1,1), 4.5);

    Matrix B = A; // copy
    EXPECT_TRUE(A == B);

    B(1,1) = 0.0;
    EXPECT_FALSE(A == B);
}

// transpose vs Armadillo

TEST(MatrixBasics, TransposeMatchesArmadillo) {
    Matrix A = Matrix::Random(2, 3);

    Matrix At_mat_lib = A.transpose();
    arma::mat A_ref = to_arma(A);
    arma::mat At_ref = A_ref.t();

    double max_err = max_abs_error(At_mat_lib, At_ref);
    std::cout << "[Basics] Transpose 2x3 -> 3x2  max_abs_error = "
              << max_err << "\n";

    EXPECT_TRUE(mats_close(At_mat_lib, At_ref));
}

// symmetry check

TEST(MatrixBasics, IsSymmetricBehavior) {
    Matrix S(3, 3);
    S(0,0) = 1.0;  S(0,1) = 2.0;   S(0,2) = -1.0;
    S(1,0) = 2.0;  S(1,1) = 0.5;   S(1,2) = 3.0;
    S(2,0) = -1.0; S(2,1) = 3.0;   S(2,2) = 4.0;

    EXPECT_TRUE(S.is_symmetric(1e-12));

    // break symmetry by a bit above tolerance to check it 
    // doesn't regiter as symmetric
    S(0,2) = -1.0 + 1e-3; // diff = 1e-3

    EXPECT_FALSE(S.is_symmetric(1e-12));
}

#include <gtest/gtest.h>
#include "Matrix.hpp"

// A basic test
TEST(MatrixTests, ConstructorInitializesToZero) {
    Matrix A(3, 3);
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            EXPECT_DOUBLE_EQ(A(i, j), 0.0);
        }
    }
}

TEST(MatrixTests, SymmetryCheck) {
    Matrix A(2, 2);
    A(0,0) = 1.0; A(0,1) = 2.0;
    A(1,0) = 2.0; A(1,1) = 3.0;

    // Your "is symmetric" helper:
    EXPECT_TRUE(A.is_symmetric());
}

// Required main function (only if you don’t link gtest_main)
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

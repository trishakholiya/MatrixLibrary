#include <gtest/gtest.h>
#include <armadillo>
#include <iostream>
#include "matrix.h"
#include "test_helpers.hpp"
#include <fstream>

// set output file for accuracy data
static const char* EIGSYM_OUT = "eigsym_accuracy.txt";

inline arma::vec to_arma_vec(const vec& v) {
    arma::vec out(v.size());
    for (arma::uword i = 0; i < out.n_elem; ++i) {
        out(i) = v[i];
    }
    return out;
}

// single matrix sanity check
TEST(MatrixEigsym, EigsymMatchesArmadilloSmall) {
    int n = 4;
    Matrix S = random_symmetric_matrix(n);

    // MatrixLibrary implementation
    EigsymResult mat_lib = S.eigsym();

    // compare with Armadillo
    arma::mat S_ref = to_arma(S);
    arma::vec evals_ref;
    arma::mat evecs_ref;
    arma::eig_sym(evals_ref, evecs_ref, S_ref);  // assumes S is symmetric

    // eigenvalues (sort for consistent comparison)
    arma::vec evals_mat_lib_sorted = arma::sort(to_arma_vec(mat_lib.eigenvalues));
    arma::vec evals_ref_sorted     = arma::sort(evals_ref);
    double max_eval_err = arma::max(arma::abs(evals_mat_lib_sorted - evals_ref_sorted));

    // eigenvectors
    arma::mat V_mat_lib = to_arma(mat_lib.eigenvectors);

    // reconstruction uses UNSORTED eigenvalues (paired with eigenvectors)
    arma::vec evals_mat_lib_unsorted = to_arma_vec(mat_lib.eigenvalues);
    arma::mat S_recon = V_mat_lib * arma::diagmat(evals_mat_lib_unsorted) * V_mat_lib.t();
    double recon_err = arma::norm(S_recon - S_ref, "fro");

    // check orthogonality: V^T V ≈ I
    arma::mat I = arma::eye(n, n);
    double orth_err = arma::norm(V_mat_lib.t() * V_mat_lib - I, "fro");

    std::cout << "[Eigsym small] n = " << n
              << "  max_eval_err = " << max_eval_err
              << "  recon_err = " << recon_err
              << "  orth_err = " << orth_err << "\n";

    // write metrics to txt file
    std::ofstream out(EIGSYM_OUT, std::ios::app);
    out << n << " "
        << max_eval_err << " "
        << recon_err << " "
        << orth_err << "\n";

    // can tune these tolerances based on floating-point error if wanted
    EXPECT_LT(max_eval_err, 1e-10);
    EXPECT_LT(recon_err,   1e-10);
    EXPECT_LT(orth_err,    1e-10);
}

// #### parameterized tests with different matrix sizes #### //

class EigsymSizeTest : public ::testing::TestWithParam<int> {};

TEST_P(EigsymSizeTest, EigsymMatchesArmadilloForSize) {
    int n = GetParam();

    Matrix S = random_symmetric_matrix(n);

    EigsymResult mat_lib = S.eigsym();

    arma::mat S_ref = to_arma(S);
    arma::vec evals_ref;
    arma::mat evecs_ref;
    arma::eig_sym(evals_ref, evecs_ref, S_ref);

    // eigenvalues (sort for consistent comparison)
    arma::vec evals_mat_lib_sorted = arma::sort(to_arma_vec(mat_lib.eigenvalues));
    arma::vec evals_ref_sorted     = arma::sort(evals_ref);
    double max_eval_err = arma::max(arma::abs(evals_mat_lib_sorted - evals_ref_sorted));

    // eigenvectors
    arma::mat V_mat_lib = to_arma(mat_lib.eigenvectors);

    // reconstruction uses UNSORTED eigenvalues (paired with eigenvectors)
    arma::vec evals_mat_lib_unsorted = to_arma_vec(mat_lib.eigenvalues);
    arma::mat S_recon = V_mat_lib * arma::diagmat(evals_mat_lib_unsorted) * V_mat_lib.t();
    double recon_err = arma::norm(S_recon - S_ref, "fro");

    // orthogonality: V^T V ≈ I
    double orth_err  = arma::norm(V_mat_lib.t() * V_mat_lib - arma::eye(n,n), "fro");

    std::cout << "[Eigsym] n = " << n
              << "  max_eval_err = " << max_eval_err
              << "  recon_err = " << recon_err
              << "  orth_err = " << orth_err << "\n";

    // append metrics to file
    std::ofstream out(EIGSYM_OUT, std::ios::app);
    out << n << " "
        << max_eval_err << " "
        << recon_err << " "
        << orth_err << "\n";

    // can loosen slightly as n increases if needed
    EXPECT_LT(max_eval_err, 1e-8);
    EXPECT_LT(recon_err,    1e-8);
    EXPECT_LT(orth_err,     1e-8);
}

// sizes to test
INSTANTIATE_TEST_SUITE_P(
    EigsymSizes,
    EigsymSizeTest,
    ::testing::Values(2, 3, 4, 5, 8, 10)
);

// 1x1 matrix: trivial eigenproblem
TEST(MatrixEigsym, Eigsym1x1Analytic) {
    Matrix S(1,1);
    double lambda = 3.14;
    S(0,0) = lambda;

    EigsymResult res = S.eigsym();

    ASSERT_EQ(res.eigenvalues.size(), 1u);
    EXPECT_NEAR(res.eigenvalues[0], lambda, 1e-14);

    // eigenvector should be +/- 1
    ASSERT_EQ(res.eigenvectors.get_num_rows(), 1);
    ASSERT_EQ(res.eigenvectors.get_num_cols(), 1);
    double v = res.eigenvectors(0,0);
    EXPECT_NEAR(std::abs(v), 1.0, 1e-14);
}

// diagonal matrix: eigenvalues are just the diagonal entries
TEST(MatrixEigsym, EigsymDiagonalMatrix) {
    int n = 4;
    Matrix S(n, n);
    std::vector<double> diag_vals = {1.0, 2.0, 5.0, -3.0};

    // build diagonal matrix
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            S(i,j) = (i == j) ? diag_vals[i] : 0.0;
        }
    }

    EigsymResult res = S.eigsym();

    // sort eigenvalues and the true diagonal values and compare
    arma::vec evals_mat_lib = arma::sort(to_arma_vec(res.eigenvalues));
    arma::vec evals_true(n);
    for (int i = 0; i < n; ++i) {
        evals_true(i) = diag_vals[i];
    }
    evals_true = arma::sort(evals_true);

    double max_eval_err = arma::max(arma::abs(evals_mat_lib - evals_true));
    EXPECT_LT(max_eval_err, 1e-12);

    // also check reconstruction and orthogonality
    arma::mat V = to_arma(res.eigenvectors);
    arma::vec evals_unsorted = to_arma_vec(res.eigenvalues);
    arma::mat S_recon = V * arma::diagmat(evals_unsorted) * V.t();
    double recon_err = arma::norm(S_recon - to_arma(S), "fro");
    double orth_err  = arma::norm(V.t() * V - arma::eye(n,n), "fro");

    EXPECT_LT(recon_err, 1e-12);
    EXPECT_LT(orth_err,  1e-12);
}

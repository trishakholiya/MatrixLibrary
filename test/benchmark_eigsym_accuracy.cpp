#include <iostream>
#include <fstream>
#include <armadillo>
#include "matrix.h"
#include "test_helpers.hpp"   // for max_abs_error, to_arma()

// helper to generate symmetric random matrix
Matrix random_symmetric(int n) {
    Matrix A = Matrix::Random(n, n);
    Matrix S(n, n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            S(i,j) = 0.5 * (A(i,j) + A(j,i));
    return S;
}

int main() {
    std::ofstream out("eigsym_accuracy_data.txt");

    std::vector<int> sizes = {2, 3, 4, 5, 8, 10, 20, 30, 40, 50};

    for (int n : sizes) {
        for (int trial = 0; trial < 40; ++trial) {   // 40 matrices per size

            Matrix S = random_symmetric(n);

            // MatrixLibrary eigensolver
            EigsymResult mat_lib = S.eigsym();

            // armadillo as reference eigensolver
            arma::mat S_ref = to_arma(S);
            arma::vec evals_ref;
            arma::mat evecs_ref;
            arma::eig_sym(evals_ref, evecs_ref, S_ref);

            // compute sorted reference eigenvalues
            arma::vec evals_mat_lib(mat_lib.eigenvalues.size());
            for (size_t i = 0; i < evals_mat_lib.n_elem; i++)
                evals_mat_lib(i) = mat_lib.eigenvalues[i];
            evals_mat_lib = arma::sort(evals_mat_lib);
            evals_ref = arma::sort(evals_ref);

            double eig_err = arma::max(arma::abs(evals_mat_lib - evals_ref));

            // reconstruction error
            arma::mat V_mat_lib = to_arma(mat_lib.eigenvectors);
            arma::mat S_recon = V_mat_lib * arma::diagmat(evals_mat_lib) * V_mat_lib.t();
            double recon_err = arma::norm(S_recon - S_ref, "fro");

            // orthogonality error
            double orth_err = arma::norm(V_mat_lib.t() * V_mat_lib - arma::eye(n, n), "fro");

            // log format: n eig_err recon_err orth_err
            out << n << " " << eig_err << " " << recon_err << " " << orth_err << "\n";
        }
    }

    return 0;
}

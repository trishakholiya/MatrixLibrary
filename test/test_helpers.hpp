#pragma once

#include <armadillo>
#include "matrix.h"
#include <fstream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <random>

inline arma::mat to_arma(const Matrix& M) {
    arma::mat A(M.get_num_rows(), M.get_num_cols());
    for (int i = 0; i < M.get_num_rows(); ++i) {
        for (int j = 0; j < M.get_num_cols(); ++j) {
            A(i, j) = M(i, j);
        }
    }
    return A;
}

inline Matrix from_arma(const arma::mat& A) {
    vec data;
    data.reserve(A.n_rows * A.n_cols);
    for (arma::uword i = 0; i < A.n_rows; ++i) {
        for (arma::uword j = 0; j < A.n_cols; ++j) {
            data.push_back(A(i, j));
        }
    }
    return Matrix(data, static_cast<int>(A.n_rows),
                         static_cast<int>(A.n_cols));
}

inline bool mats_close(const Matrix& M,
                       const arma::mat& A,
                       double atol = 1e-12,
                       double rtol = 1e-12) {
    if (M.get_num_rows() != static_cast<int>(A.n_rows) ||
        M.get_num_cols() != static_cast<int>(A.n_cols)) {
        return false;
    }
    for (int i = 0; i < M.get_num_rows(); ++i) {
        for (int j = 0; j < M.get_num_cols(); ++j) {
            double x = M(i, j);
            double y = A(i, j);
            double diff = std::abs(x - y);
            double scale = std::max({1.0, std::abs(x), std::abs(y)});
            if (diff > atol + rtol * scale) return false;
        }
    }
    return true;
}

inline double max_abs_error(const Matrix& M, const arma::mat& A) {
    if (M.get_num_rows() != static_cast<int>(A.n_rows) ||
        M.get_num_cols() != static_cast<int>(A.n_cols)) {
        throw std::runtime_error("Shape mismatch in max_abs_error");
    }

    double max_err = 0.0;

    for (int i = 0; i < M.get_num_rows(); ++i) {
        for (int j = 0; j < M.get_num_cols(); ++j) {
            double diff = std::abs(M(i, j) - A(i, j));
            if (diff > max_err) {
                max_err = diff;
            }
        }
    }

    return max_err;
}

// max |v_i - ref_i| for eigenvalues
inline double max_abs_error_vec(const vec& v, const arma::vec& ref) {
    if (v.size() != ref.n_elem) {
        throw std::runtime_error("Size mismatch in max_abs_error_vec");
    }
    double max_err = 0.0;
    for (arma::uword i = 0; i < ref.n_elem; ++i) {
        double diff = std::abs(v[i] - ref(i));
        if (diff > max_err) max_err = diff;
    }
    return max_err;
}

inline Matrix random_symmetric_matrix(int n) {
    Matrix A = Matrix::Random(n, n);
    Matrix S(n, n);

    // S = 0.5 * (A + A^T)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            S(i,j) = 0.5 * (A(i,j) + A(j,i));
        }
    }
    return S;
}

// append one row to a CSV file for accuracy tables
inline void append_csv(
    const std::string& filename,
    const std::string& test,
    const std::string& op,
    int rowsA, int colsA,
    int rowsB, int colsB,
    double max_abs_error
) {
    std::ofstream out(filename, std::ios::app);
    out << test << ","
        << op << ","
        << rowsA << ","
        << colsA << ","
        << rowsB << ","
        << colsB << ","
        << std::setprecision(16) << max_abs_error
        << "\n";
}
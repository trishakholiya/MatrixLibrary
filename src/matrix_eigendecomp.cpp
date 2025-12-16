#include "matrix.h"
#include "helper_func.cpp"
#include <cmath>
#include <stdexcept>
#include <limits>
#include <algorithm> // for std::sort
#include <numeric> // for std::iota

/* 
Reduce a real symmetric matrix to tridiagonal form using Householder reflections.
If yesvecs == true, also accumulate the orthogonal Householder matrix Q.
Returns:
  d : diagonal entries of tridiagonal matrix
  e : off-diagonal entries of tridiagonal matrix
  Q_house : accumulated Householder transform (if yesvecs) 
*/


TridiagonalResult Matrix::householder_tridiagonalize(bool yesvecs) const {
    TridiagonalResult result;
    int l, k, j, i;
    int n = num_rows;
    Matrix z = *this; // working copy of matrix
    vec e(n), d(n); // d = diagonal, e = off-diagonal of tridiagonal matrix
    double scale, hh, h, g, f;

    // loop of rows/columns starting at the bottom
    for (i = n - 1; i > 0; i--) {
      l = i - 1;
      h = scale = 0.0;

      // compute scaling factor
      if (l > 0) {
        for (k = 0; k < i; k++)
          scale += std::abs(z(i, k));

        // if scale is zero then the column is already populated with zeros
        if (scale == 0.0) {
          e[i] = z(i, l);
        } else {
          // normalize row by scaling factor and compute the squared norm
          for (k = 0; k < i; k++) {
            z(i, k) /= scale;
            h += z(i, k) * z(i, k);
          }

          // choose reflector direction
          f = z(i, l);
          g = (f >= 0.0 ? -std::sqrt(h) : std::sqrt(h));
          e[i] = scale * g; // store off-diagonal element
          // update reflector vector
          h -= f * g;
          z(i, l) = f - g;
          f = 0.0;

          // apply similarity transformation
          for (j = 0; j < i; j ++) {
            if (yesvecs)
              // store for eigenvector accumulation
              z(j, i) = z(i, j) / h;
            g = 0.0;

            // compute dot products for transformation
            for (k = 0; k < j+1; k++)
              g += z(j, k) * z(i, k);
            for (k = j + 1; k < i; k++)
              g += z(k, j) * z(i, k);
            e[j] = g / h;
            f += e[j] * z(i, j);
        }

        // correction term
        hh = f / (h + h);

        // apply rank 2 update
        for (j = 0; j < i; j++) {
          f = z(i, j);
          e[j] = g = e[j] - hh * f;

          for (k = 0; k < j + 1; k++)
            z(j, k) -= (f * e[k] + g * z(i, k));
        }
      }
      } else
          e[i] = z(i, l); // edge case: single element remains
        d[i] = h; // store disgonal element
    }
    // initialize first entries
    if (yesvecs)
      d[0] = 0.0;
    e[0] = 0.0;

    // accumulate householder transformations in Q if yesvecs == true
    for (i = 0; i < n; i++) {
      if (yesvecs) {
        if (d[i] != 0.0) {
          // apply transformations to earlier columns
          for (j = 0; j < i; j++) {
            g = 0.0;
            for (k = 0; k < i; k++) 
              g += z(i, k) * z(k, j);
            for (k = 0; k < i; k++)
              z(k, j) -= g * z(k, i);
          }
        }
        
        // form Q_house (orthogonal matrix)
        d[i] = z(i, i);
        z(i, i) = 1.0;
        for (j = 0; j < i; j++)
          z(j, i) = z(i, j) = 0.0;

      } else {
        // only extract diagonal
        d[i] = z(i, i);
      }        
    }

    if (yesvecs) {
      result.Q_house = z;
    }

    result.d = d;
    result.e = e;

    return result;
}

/*
 Implicit QL algorithm for symmetric tridiagonal matrices.
 Input:
   d : diagonal entries
   e : off-diagonal entries
 Output:
   eigenvalues in d
   Q_ql : orthogonal matrix of eigenvectors
*/

QLEigenResult Matrix::QL(std::vector<double> d, std::vector<double> e) const {
  QLEigenResult result;
  int n = d.size();
  int m, l, iter, i, k;
  double s, r, p, g, f, dd, c, b;
  const double eps=std::numeric_limits<double>::epsilon();

  Matrix z = Matrix::Identity(n); // to store eigenvectors

  // shift e so that e[i] is subdiagonal between d[i] and d[i+1]
  for (i = 1; i < n; i++) {
    e[i-1] = e[i];
  }
  e[n-1] = 0.0;

  // loop over eigenvalues
  for ( l=0; l<n; l++ ) {
    iter = 0;
    do {
      // find small subdiagonal element for deflation
      for ( m=l; m<n-1; m++ ) {
        dd = std::abs(d[m]) + std::abs(d[m+1]);
        if ( std::abs(e[m]) <= eps*dd ) break;
      }
      if ( m!=l ) {
        if ( iter++ == 30 ) throw std::runtime_error("Too many iterations in tqli");
        // wilkinson shift
        g = (d[l+1] - d[l]) / (2.0 * e[l]);
        r = pythag(g, 1.0);
        g = d[m] - d[l] + e[l]/(g + SIGN(r,g));
        s = 1.0;
        c = 1.0;
        p = 0.0;

        // implicit QL iteration
        for (i = m-1; i >= l; i--) {
          f = s * e[i];
          b = c * e[i];

          e[i+1] = (r=pythag(f, g));
          if ( r == 0 ) {
            d[i+1] -= p;
            e[m] = 0.0;
            break;
          }
          s = f / r;
          c = g / r;
          g = d[i+1]-p;
          r =  (d[i] - g)*s+2.0*c*b;
          d[i+1]  = g + (p=s*r);
          g = c*r - b;
          
          // apply rotation to eigenvector matrix
          for ( k=0; k<n; k++ ) {
            f = z(k, i+1);
            z(k, i+1) = s*z(k, i)+c*f;
            z(k, i) = c*z(k, i)-s*f;
          }

        }

        if ( r== 0.0 && i >= l) continue;

        // update diagonal and off-diagonal
        d[l] -= p;
        e[l] = g;
        e[m] = 0.0;
      }
    } while ( m != l );
  }

  result.eigenvalues = d;
  result.Q_ql = z;
  return result;
}

// Compute eigenvalues and eigenvectors of a real symmetric matrix.
// Pipeline:
//   Householder tridiagonalization -> QL eigensolver -> combine transforms

EigsymResult Matrix::eigsym() const {
  // make sure matrix is square
  if (num_rows != num_cols) {
    throw InvalidMatrixSize("householder_tridiagonalize requires a square matrix");
  }

  // make sure matrix is symmetric
  if (!is_symmetric(1e-8)) {
    throw InvalidMatrixSize("Matrix must be symmetric for eigsym()");
  }
    
  // reduce matrix to tridiagonal form
  TridiagonalResult tri = householder_tridiagonalize(true);

  // apply QL to solve for eigenvalues and eigenvectors
  QLEigenResult ql = QL(tri.d, tri.e);

  // combine householder and QL eigenvectors
  Matrix P = tri.Q_house * ql.Q_ql;

  vec eigenvalues = ql.eigenvalues; // copy so eigenvalues can be reordered to match Armadillo
  int n = static_cast<int>(eigenvalues.size());

  // build index array
  std::vector<int> idx(n);
  std::iota(idx.begin(), idx.end(), 0);


  // sort indices by eigenvalue (ascending)
  std::sort(idx.begin(), idx.end(), [&](int i, int j) {
    return eigenvalues[i] < eigenvalues[j];
  });

  // build sorted eigenvalues and eigenvectors
  vec eigenvalues_sorted(n);
  Matrix P_sorted(n, n);

  for (int k = 0; k < n; ++k) {
    int j = idx[k];  // original eigenpair index
    eigenvalues_sorted[k] = eigenvalues[j];

    // copy column j of P into column k of P_sorted
    for (int row = 0; row < n; ++row) {
      P_sorted(row, k) = P(row, j);
    }
  }

  EigsymResult result;
  result.eigenvalues  = std::move(eigenvalues_sorted);
  result.eigenvectors = std::move(P_sorted);

  return result;
}
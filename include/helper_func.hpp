#pragma once

/**
 * @brief Return a value equivalent to |a| * sign(b)
 * 
 * Helper function for eigenvalue and eigenvector calculation.
 * Adapted from Numerical Recipes.
 */
double SIGN(double a, double b);

/**
 * @brief Computes sqrt(a^2 + b^2) without destructive overflow or underflow
 * 
 * Helper function for eigenvalue and eigenvector calculation.
 * Adapted from Numerical Recipes.
 */
double pythag(double a, double b);

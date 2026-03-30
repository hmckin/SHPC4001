/*
 *  Written by: Harry McKinney Student #: 25258748
 *  Mar 30, 2026
 *  Assignment 3 - Question 2
 *  Implements the Inverse Power Method to find the smallest magnitude eigenvalue of a 3x3 complex matrix.
 * 
 *  Outputs in the terminal and creates a convergence csv.
 */

#include <stdio.h>
#include <complex.h>
#include <math.h>

typedef double complex complex_t;

complex_t determinant3x3(complex_t A[3][3]) {
    return A[0][0] * (A[1][1] * A[2][2] - A[1][2] * A[2][1]) -
           A[0][1] * (A[1][0] * A[2][2] - A[1][2] * A[2][0]) +
           A[0][2] * (A[1][0] * A[2][1] - A[1][1] * A[2][0]);
}

void invert3x3(complex_t A[3][3], complex_t inv[3][3]) {
    complex_t det = determinant3x3(A);
    if (cabs(det) < 1e-15) {
        printf("Matrix is singular.\n");
        return;
    }

    // Adjugate matrix elements (cofactors transposed)
    inv[0][0] = (A[1][1] * A[2][2] - A[1][2] * A[2][1]) / det;
    inv[0][1] = (A[0][2] * A[2][1] - A[0][1] * A[2][2]) / det;
    inv[0][2] = (A[0][1] * A[1][2] - A[0][2] * A[1][1]) / det;

    inv[1][0] = (A[1][2] * A[2][0] - A[1][0] * A[2][2]) / det;
    inv[1][1] = (A[0][0] * A[2][2] - A[0][2] * A[2][0]) / det;
    inv[1][2] = (A[1][0] * A[0][2] - A[0][0] * A[1][2]) / det;

    inv[2][0] = (A[1][0] * A[2][1] - A[1][1] * A[2][0]) / det;
    inv[2][1] = (A[2][0] * A[0][1] - A[0][0] * A[2][1]) / det;
    inv[2][2] = (A[0][0] * A[1][1] - A[0][1] * A[1][0]) / det;
}

// Matrix-vector multiplication
void mat_vec_mul(complex_t M[3][3], complex_t v[3], complex_t res[3]) {
    for (int i = 0; i < 3; i++) {
        res[i] = 0;
        for (int j = 0; j < 3; j++) {
            res[i] += M[i][j] * v[j];
        }
    }
}

// Vector norm (Euclidean)
double vector_norm(complex_t v[3]) {
    double sum = 0;
    for (int i = 0; i < 3; i++) {
        sum += creal(v[i] * conj(v[i]));
    }
    return sqrt(sum);
}

int main() {

    complex_t A[3][3] = {
        {4.0 + 0*I, 0.0 - 1*I, 2.0 + 0*I},
        {0.0 + 1*I, 2.0 + 0*I, 2.0 + 7*I},
        {2.0 + 0*I, 2.0 - 7*I, -2.0 + 0*I}
    };

    complex_t A_inv[3][3];
    invert3x3(A, A_inv);

    // Initial guess for the eigenvector 
    complex_t v[3] = {1.0, 0, 0};
    double n0 = vector_norm(v);
    for (int i = 0; i < 3; i++) v[i] /= n0;

    complex_t next_v[3];
    double diff = 1.0;
    double tol = 1e-12;
    int max_iter = 100;
    int iter = 0;
    double lambda_exact = 3.1896027470; //determined in part a

    FILE *fp = fopen("convergence.csv", "w");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return 1;
    }
    fprintf(fp, "iteration,error\n");

    while (diff > tol && iter < max_iter) {
        mat_vec_mul(A_inv, v, next_v);
        
        double norm = vector_norm(next_v);
        double lambda_int = 1.0 / norm;
        double error = fabs(lambda_int - lambda_exact);
        fprintf(fp, "%d,%.15e\n", iter, error);

        for (int i = 0; i < 3; i++) {
            next_v[i] /= norm;
        }

        // Convergence check: absolute difference of normalized vectors
        diff = 0;
        for (int i = 0; i < 3; i++) {
            diff += cabs(v[i] - next_v[i]);
        }

        for (int i = 0; i < 3; i++) {
            v[i] = next_v[i];
        }
        iter++;
    }
    fclose(fp);

    // Eigenvalue of A_inv (mu)
    // mu = (v_dagger * A_inv v)
    complex_t Av[3];
    mat_vec_mul(A_inv, v, Av);
    complex_t mu = 0;
    for (int i = 0; i < 3; i++) {
        mu += conj(v[i]) * Av[i];
    }

    complex_t lambda_min = 1.0 / mu;

    printf("Converged in %d iterations.\n", iter);
    printf("Smallest magnitude eigenvalue: %.10f %+.10fi\n", creal(lambda_min), cimag(lambda_min));
    printf("Magnitude: %.10f\n", cabs(lambda_min));

    return 0;
}

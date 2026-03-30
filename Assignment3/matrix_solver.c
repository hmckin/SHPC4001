/*
 *  Written by: Harry McKinney Student #: 25258748
 *  Mar 30, 2026
 *  Assignment 3 - Question 2
 *  Solves the eigensystem of a tridiagonal banded matrix using LAPACK's dsbev function.
 * 
 *  Output is 2 CSV files: eigenvalues.csv and eigenvectors.csv
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lapacke.h>

void construct_banded_matrix(int N, double ***L_ptr) {
    // For a tridiagonal matrix, number of super-diagonals kd = 1.
    int kd = 1;
    int rows = kd + 1;

    // Allocating 2D array
    *L_ptr = (double **)malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++) {
        (*L_ptr)[i] = (double *)malloc(N * sizeof(double));
        // Initialize with zeros
        memset((*L_ptr)[i], 0, N * sizeof(double));
    }

    double **L = *L_ptr;

    // Main diagonal is 2.0 (Row 1)
    // Super-diagonal is -1.0 (Row 0)
    for (int j = 0; j < N; j++) {
        L[1][j] = 2.0;
        if (j > 0) {
            L[0][j] = -1.0;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <N>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        printf("N must be a positive integer.\n");
        return 1;
    }

    double **L_banded_2d = NULL;
    construct_banded_matrix(n, &L_banded_2d);

    // contiguous block for the LAPACKE call.
    double *ab = (double *)malloc(2 * n * sizeof(double));
    for (int j = 0; j < n; j++) {
        ab[0 * n + j] = L_banded_2d[0][j];
        ab[1 * n + j] = L_banded_2d[1][j];
    }

    double *w = (double *)malloc(n * sizeof(double));      // Eigenvalues
    double *z = (double *)malloc(n * n * sizeof(double));  // Eigenvectors
    int ldz = n;

    int info = LAPACKE_dsbev(LAPACK_ROW_MAJOR, 'V', 'U', n, 1, ab, n, w, z, ldz);

    if (info == 0) {
        // Save eigenvalues
        FILE *feig = fopen("eigenvalues.csv", "w");
        fprintf(feig, "index,eigenvalue\n");
        for (int i = 0; i < n; i++) {
            fprintf(feig, "%d,%.15e\n", i, w[i]);
        }
        fclose(feig);

        // Save eigenvectors
        FILE *fvec = fopen("eigenvectors.csv", "w");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                // z is row-major, so z[i*n + j] is component i of eigenvector j
                fprintf(fvec, "%.15e%s", z[i * n + j], (j == n - 1) ? "" : ",");
            }
            fprintf(fvec, "\n");
        }
        fclose(fvec);

        printf("Successfully computed eigensystem for N=%d.\n", n);
        printf("Results saved to eigenvalues.csv and eigenvectors.csv.\n");
    } else {
        printf("LAPACKE_dsbev failed with info = %d\n", info);
    }

    // Cleanup
    free(ab);
    free(w);
    free(z);
    for (int i = 0; i < 2; i++) free(L_banded_2d[i]);
    free(L_banded_2d);

    return 0;
}

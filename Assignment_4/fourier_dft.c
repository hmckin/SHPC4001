/*
 *  Written by: Harry McKinney Student #: 25258748
 *  April 13, 2026
 *  Assignment 4 - Question 1 & 2
 *  Implements Fourier series expansions (triangle and sawtooth waves),
 *  analyzes convergence, applies Lanczos sigma factors, and implements 
 *  a naive Discrete Fourier Transform (DFT).
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#ifndef M_PI
#endif

// Q1.1 & 1.2 Fourier Series

double triangle_wave(double x, double L) {
    return 1.0 - fabs(4.0 * x / L);
}

double sawtooth_wave(double x, double L) {
    return 2.0 * x / L;
}

double triangle_ak(int k) {
    if (k == 0) return 0.0;
    return (4.0 * (1.0 - pow(-1, k))) / (k * k * M_PI * M_PI);
}

double sawtooth_bk(int k) {
    if (k == 0) return 0.0;
    return (2.0 * pow(-1, k + 1)) / (k * M_PI);
}

double fourier_series(double x, double L, int n, int type) {
    double sum = 0.0; // a0/2 is 0 for both
    for (int k = 1; k <= n; k++) {
        if (type == 0) { // Triangle
            sum += triangle_ak(k) * cos(2.0 * M_PI * k * x / L);
        } else { // Sawtooth
            sum += sawtooth_bk(k) * sin(2.0 * M_PI * k * x / L);
        }
    }
    return sum;
}

// Q1.3 Lanczos Sigma Factors

double sinc(double x) {
    if (fabs(x) < 1e-12) return 1.0;
    return sin(M_PI * x) / (M_PI * x);
}

double fourier_lanczos(double x, double L, int n) {
    double sum = 0.0;
    for (int k = 1; k <= n; k++) {
        double sigma = sinc((double)k / (n + 1));
        sum += sigma * sawtooth_bk(k) * sin(2.0 * M_PI * k * x / L);
    }
    return sum;
}

// Q2 Naive DFT 

double complex *naive_dft(const double complex *fin, int N) {
    double complex *fout = malloc(N * sizeof(double complex));
    if (fout == NULL) return NULL;

    for (int m = 0; m < N; m++) {
        fout[m] = 0.0 + 0.0 * I;
        for (int n = 0; n < N; n++) {
            double angle = -2.0 * M_PI * m * n / N;
            fout[m] += fin[n] * (cos(angle) + I * sin(angle));
        }
    }
    return fout;
}

double complex *naive_idft(const double complex *fin, int N) {
    double complex *fout = malloc(N * sizeof(double complex));
    if (fout == NULL) return NULL;

    for (int n = 0; n < N; n++) {
        fout[n] = 0.0 + 0.0 * I;
        for (int m = 0; m < N; m++) {
            double angle = 2.0 * M_PI * m * n / N;
            fout[n] += fin[m] * (cos(angle) + I * sin(angle));
        }
        fout[n] /= (double)N;
    }
    return fout;
}

int main() {
    double L = 1.0;
    int terms[] = {5, 10, 20};
    int n_points = 1000;

    // Q1.1 Partial Sums Data
    FILE *f1 = fopen("csv/fourier_sums.csv", "w");
    fprintf(f1, "x,f_tri,tri5,tri10,tri20,f_saw,saw5,saw10,saw20\n");
    for (int i = 0; i < n_points; i++) {
        double x = -L/2.0 + i * L / (n_points - 1);
        fprintf(f1, "%f,%f,%f,%f,%f,%f,%f,%f,%f\n", x, 
                triangle_wave(x, L),
                fourier_series(x, L, 5, 0), fourier_series(x, L, 10, 0), fourier_series(x, L, 20, 0),
                sawtooth_wave(x, L),
                fourier_series(x, L, 5, 1), fourier_series(x, L, 10, 1), fourier_series(x, L, 20, 1));
    }
    fclose(f1);

    // Q1.2 Convergence Data
    FILE *f2 = fopen("csv/fourier_convergence.csv", "w");
    fprintf(f2, "n,err_tri_0,err_tri_0125,err_saw_0,err_saw_0125\n");
    for (int n = 2; n <= 200; n += 2) {
        double x1 = 0.0, x2 = 0.125;
        double err_tri_0 = fabs(triangle_wave(x1, L) - fourier_series(x1, L, n, 0));
        double err_tri_0125 = fabs(triangle_wave(x2, L) - fourier_series(x2, L, n, 0));
        double err_saw_0 = fabs(sawtooth_wave(x1, L) - fourier_series(x1, L, n, 1));
        double err_saw_0125 = fabs(sawtooth_wave(x2, L) - fourier_series(x2, L, n, 1));
        fprintf(f2, "%d,%e,%e,%e,%e\n", n, err_tri_0, err_tri_0125, err_saw_0, err_saw_0125);
    }
    fclose(f2);

    // Q1.3 Lanczos Sigma Factors Data
    FILE *f3 = fopen("csv/fourier_lanczos.csv", "w");
    fprintf(f3, "x,f_saw,saw5,saw10,saw20,lan5,lan10,lan20\n");
    for (int i = 0; i < n_points; i++) {
        double x = -L/2.0 + i * L / (n_points - 1);
        fprintf(f3, "%f,%f,%f,%f,%f,%f,%f,%f\n", x, sawtooth_wave(x, L),
                fourier_series(x, L, 5, 1), fourier_series(x, L, 10, 1), fourier_series(x, L, 20, 1),
                fourier_lanczos(x, L, 5), fourier_lanczos(x, L, 10), fourier_lanczos(x, L, 20));
    }
    fclose(f3);

    // Q2 Naive DFT Data
    int N_vals[] = {32, 128};
    for (int k = 0; k < 2; k++) {
        int N = N_vals[k];
        double complex *fin = malloc(N * sizeof(double complex));
        for (int i = 0; i < N; i++) {
            double x = 4.0 * M_PI * i / N;
            fin[i] = exp(cos(x)) + 0.0 * I;
        }

        double complex *fout = naive_dft(fin, N);
        double complex *frec = naive_idft(fout, N);

        char fname[60];
        sprintf(fname, "csv/dft_results_N%d.csv", N);
        FILE *f4 = fopen(fname, "w");
        fprintf(f4, "i,x,real_in,imag_in,mag_dft,real_rec,imag_rec\n");
        for (int i = 0; i < N; i++) {
            double x = 4.0 * M_PI * i / N;
            double mag = cabs(fout[i]);
            fprintf(f4, "%d,%f,%f,%f,%f,%f,%f\n", i, x, creal(fin[i]), cimag(fin[i]), mag, creal(frec[i]), cimag(frec[i]));
        }
        fclose(f4);

        free(fin);
        free(fout);
        free(frec);
    }

    printf("Fourier and DFT data generation complete.\n");
    return 0;
}

/*
 *  Written by: Harry McKinney Student #: 25258748
 *  April 14, 2026
 *  Assignment 4 - Question 3
 *  Calculates the evolution of a Gaussian wavepacket and its Fourier transform
 *  using the FFTW library.
 * 
 *  run using: gcc fft_packet.c -o fft -I/opt/homebrew/include -L/opt/homebrew/lib -lfftw3
 */

#include <stdlib.h>
#include <math.h>
#include <fftw3.h>
#include <complex.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void run_fft_packet(double t) {
    double x_min = -20.0;
    double x_max = 20.0;
    double dx = 0.1;
    int N = (int)((x_max - x_min) / dx);
    
    fftw_complex *in, *out;
    fftw_plan p;

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    
    double complex prefactor = 1.0 / csqrt(sqrt(M_PI) * (1.0 + I * t));
    
    for (int j = 0; j < N; j++) {
        double x = x_min + j * dx;
        double complex exponent = -(x * x) / (2.0 * (1.0 + I * t));
        double complex psi = prefactor * cexp(exponent);
        
        // Centering trick
        double shift = (j % 2 == 0) ? 1.0 : -1.0;
        
        in[j][0] = creal(psi) * shift;
        in[j][1] = cimag(psi) * shift;
    }

    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);

    char fname[60];
    sprintf(fname, "csv/wavepacket_t%.0f.csv", t);
    FILE *fp = fopen(fname, "w");
    fprintf(fp, "j,x,psi_real,psi_imag,psi_sq,fft_real,fft_imag,fft_sq\n");

    double dk = 2.0 * M_PI / (N * dx);
    // k_min = -pi / dx 
    
    for (int j = 0; j < N; j++) {
        double x = x_min + j * dx;
        
        // The centering trick shifts k=0 to the middle.
        double shift = (j % 2 == 0) ? 1.0 : -1.0;
        double psi_real = in[j][0] * shift;
        double psi_imag = in[j][1] * shift;
        double psi_sq = psi_real * psi_real + psi_imag * psi_imag;
        
        double fft_real = out[j][0] * dx / sqrt(2.0 * M_PI); // Scaling for continuous FT approximation
        double fft_imag = out[j][1] * dx / sqrt(2.0 * M_PI);
        double fft_sq = fft_real * fft_real + fft_imag * fft_imag;
        
        fprintf(fp, "%d,%f,%f,%f,%f,%f,%f,%f\n", j, x, psi_real, psi_imag, psi_sq, fft_real, fft_imag, fft_sq);
    }

    fclose(fp);
    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);
}

int main() {
    double times[] = {0.0, 1.0, 5.0};
    for (int i = 0; i < 3; i++) {
        run_fft_packet(times[i]);
    }
    printf("FFT wavepacket data generation complete.\n");
    return 0;
}

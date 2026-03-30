/*
 *  Written by: Harry McKinney Student #: 25258748
 *  Mar 30, 2026
 *  Assignment 3 - Question 3
 *  Solves the 1D time-independent Schrödinger equation for a harmonic oscillator using a bisecting shooting method.
 * 
 *  Outputs states in the terminal and 
 */

#include <stdio.h>
#include <math.h>

#define N 1000
#define L 5.0 
#define h (2.0*L/N)

double V(double x) { return 0.5 * x * x; } //harmonic oscillator in atomic units

int main() {
    double psi[N+1], res[4][N+1], energies[4], E, El, Eu;
    char filename[64];
    sprintf(filename, "schrodinger_results_h%.4f.csv", h);

    FILE *f = fopen(filename, "w");
    if (!f) return 1;

    for (int n = 0; n < 4; n++) { //n=quantum state
        El = 0; 
        Eu = 10;
        double eps = 1e-12;
        while (fabs(Eu - El) > eps) { //tolerance check
            E = (El + Eu) / 2.0;
            psi[0] = 0; 
            psi[1] = 1e-5;
            int nodes = 0;
            for (int j = 1; j < N; j++) {
                psi[j+1] = 2.0 * (1.0 + h * h * (V(-L + j * h) - E)) * psi[j] - psi[j-1]; //dervied formula
                if (psi[j] * psi[j+1] < 0) nodes++;
            }
            //nodes>n OR correct nodes but solution at end diverging (odd states approach from below -ve x -1 should > 0 and vice versa)
            if (nodes > n || (nodes == n && psi[N] * (n % 2 ? -1 : 1) < 0)) Eu = E;
            else El = E;
        }
        energies[n] = E;
        printf("State %d, Energy: %.10f\n", n, E);
        double norm = 0;
        for (int j = 0; j <= N; j++) norm += psi[j] * psi[j];
        norm = sqrt(norm * h);
        for (int j = 0; j <= N; j++) res[n][j] = psi[j] / norm;
    }

    fprintf(f, "# E0:%.10f, E1:%.10f, E2:%.10f, E3:%.10f\n", energies[0], energies[1], energies[2], energies[3]);
    fprintf(f, "x,psi0,psi1,psi2,psi3\n");
    for (int j = 0; j <= N; j++)
        fprintf(f, "%.4f,%.8f,%.8f,%.8f,%.8f\n", -L + j * h, res[0][j], res[1][j], res[2][j], res[3][j]);

    fclose(f);
    return 0;
}


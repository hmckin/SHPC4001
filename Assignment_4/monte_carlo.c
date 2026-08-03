/*
 *  Written by: Harry McKinney Student #: 25258748
 *  April 15, 2026
 *  Assignment 4 - Question 4
 *  Approximates a 3D integral using Monte Carlo integration (Method 1)
 *  and analyzes the convergence with the number of samples N.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

double f(double x, double y, double z) {
    return exp(-(x + y + z));
}

double monte_carlo_method1(int N) {
    double x_min = 0.0, x_max = 2.0;
    double y_min = 0.0, y_max = 3.0;
    double z_min = 0.0, z_max = 4.0;
    double volume = (x_max - x_min) * (y_max - y_min) * (z_max - z_min);
    
    double sum = 0.0;
    for (int i = 0; i < N; i++) {
        double x = x_min + (x_max - x_min) * ((double)rand() / RAND_MAX);
        double y = y_min + (y_max - y_min) * ((double)rand() / RAND_MAX);
        double z = z_min + (z_max - z_min) * ((double)rand() / RAND_MAX);
        sum += f(x, y, z);
    }
    
    return volume * (sum / N);
}

int main() {
    srand(time(NULL));
    
    double exact = (1.0 - exp(-2.0)) * (1.0 - exp(-3.0)) * (1.0 - exp(-4.0));
    printf("Exact value: %f\n", exact);
    
    FILE *fp = fopen("csv/monte_carlo_results.csv", "w");
    fprintf(fp, "N,I_approx,error\n");
    
    // sample points from 10 to 1,000,000, stepping n*1.5 for log-log comparison (fewer at higher N)
    for (long n = 10; n <= 1000000; n = (long)(n * 1.5)) {
        double approx = monte_carlo_method1((int)n);
        double error = fabs(exact - approx) / exact;
        fprintf(fp, "%ld,%f,%e\n", n, approx, error);
    }

    double approx_final = monte_carlo_method1(1000000);
    fprintf(fp, "1000000,%f,%e\n", approx_final, fabs(exact - approx_final) / exact);
    
    fclose(fp);
    printf("Monte Carlo data generation complete.\n");
    return 0;
}

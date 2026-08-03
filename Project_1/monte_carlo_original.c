/*
 *  Written by: Harry McKinney Student #: 25258748
 *  May 4, 2026
 *  Copy of Assignment 4 - Question 4
 *  Approximates a 3D integral using Monte Carlo integration (Method 1)
 *  and analyzes the convergence with the number of samples N.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/*
 * Integrand function: f(x, y, z) = e^-(x + y + z)
 */
double f(double x, double y, double z) {
    return exp(-(x + y + z));
}

/*
 * Performs Monte Carlo integration of f(x, y, z) over the box [0,2]x[0,3]x[0,4]
 * N: Number of random samples to take
 */
double monte_carlo_method1(int N) {
    // Define integration bounds for x, y, and z
    double x_min = 0.0, x_max = 2.0;
    double y_min = 0.0, y_max = 3.0;
    double z_min = 0.0, z_max = 4.0;
    
    // Calculate the total volume of the bounding box
    double volume = (x_max - x_min) * (y_max - y_min) * (z_max - z_min);
    
    double sum = 0.0;
    // Sample N random points within the defined box
    for (int i = 0; i < N; i++) {
        // Generate uniform random coordinates within [min, max]
        double x = x_min + (x_max - x_min) * ((double)rand() / RAND_MAX);
        double y = y_min + (y_max - y_min) * ((double)rand() / RAND_MAX);
        double z = z_min + (z_max - z_min) * ((double)rand() / RAND_MAX);
        
        // Sum the function values at these points
        sum += f(x, y, z);
    }
    
    // The integral approximation: Volume * (Average Value of Function)
    return volume * (sum / N);
}

int main() {
    // Seed random number generator with current time
    srand(time(NULL));
    
    // Analytical solution for the integral to compute error:
    // Integral(e^-x) from 0 to L is (1 - e^-L)
    double exact = (1.0 - exp(-2.0)) * (1.0 - exp(-3.0)) * (1.0 - exp(-4.0));
    printf("Exact value: %f\n", exact);
    
    // Prepare CSV file for performance and convergence logging
    FILE *fp = fopen("csv/monte_carlo_results.csv", "w");
    if (!fp) {
        perror("Failed to open csv/monte_carlo_results.csv");
        return 1;
    }
    fprintf(fp, "N,I_approx,error,runtime_s\n");
    
    // Run the simulation for a range of sample sizes N
    for (long n = 10; n <= 1000000000; n = (long)(n * 1.5)) {
        clock_t start = clock();
        double approx = monte_carlo_method1((int)n);
        clock_t end = clock();
        
        // Measure execution time in seconds
        double runtime = (double)(end - start) / CLOCKS_PER_SEC;

        // Calculate relative error compared to the exact value
        double error = fabs(exact - approx) / exact;
        
        // Log results to file and print status to terminal
        fprintf(fp, "%ld,%f,%e,%e\n", n, approx, error, runtime);
        printf("N = %12ld | Time: %.6f s | Error: %e\n", n, runtime, error);
    }

    fclose(fp);
    printf("Monte Carlo data generation complete.\n");
    return 0;
}

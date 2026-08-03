/*
 *  Written by: Harry McKinney Student #: 25258748
 *  April 30, 2026
 *  Approximates a 3D integral using Monte Carlo integration (Method 1)
 *  and analyzes the convergence with the number of samples N.
 *  Uses MPI to parallelize the computation of each iteration of N samples.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <time.h>

// Function to integrate
double f(double x, double y, double z) {
    return exp(-(x + y + z));
}

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double x_min = 0.0, x_max = 2.0;
    double y_min = 0.0, y_max = 3.0;
    double z_min = 0.0, z_max = 4.0;
    double volume = (x_max - x_min) * (y_max - y_min) * (z_max - z_min);
    double exact = (1.0 - exp(-2.0)) * (1.0 - exp(-3.0)) * (1.0 - exp(-4.0));

    // Seed uniquely per process
    srand(time(NULL) + rank);

    if (rank == 0) {
        printf("Exact value: %f\n", exact);
        printf("Strong Scaling Benchmark (MPI size: %d)\n", size);
        printf("--------------------------------------------------\n");
    }

    // Loop through same iterations
    for (long n_total = 10; n_total <= 1000000000; n_total = (long)(n_total * 1.5)) {
        
        // Divide total samples among processors
        long n_local = n_total / size;
        long remainder = n_total % size;
        if (rank < remainder) n_local++; // dish out the extra

        double start_time = MPI_Wtime(); // start for comparison

        double local_sum = 0.0;
        for (long i = 0; i < n_local; i++) {
            double x = x_min + (x_max - x_min) * ((double)rand() / RAND_MAX);
            double y = y_min + (y_max - y_min) * ((double)rand() / RAND_MAX);
            double z = z_min + (z_max - z_min) * ((double)rand() / RAND_MAX);
            local_sum += f(x, y, z);
        }

        double global_sum;
        MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        double end_time = MPI_Wtime(); // end comparison

        if (rank == 0) {
            double approx = volume * (global_sum / n_total);
            double error = fabs(exact - approx) / exact;
            double runtime = end_time - start_time;
            printf("N = %12ld | Time: %.6f s | Error: %e\n", n_total, runtime, error);
        }
    }

    MPI_Finalize();
    return 0;
}

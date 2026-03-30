/*
 *  Written by: Harry McKinney Student #: 25258748
 *  Mar 30, 2026
 *  Assignment 3 - Question 1
 *  Solves the Lorenz System using the 4th-order Runge-Kutta (RK4) numerical method.
 *  To run, accepts 2 command line arguments in the form: ./lorenz [h] [output_filename]
 *  The default is h=0.01 and filename="results.csv"
 * 
 *  Output is a csv.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 3

void f(double t, const double y[N], double dydt[N])
{
    (void)t;
    const double sigma = 10.0;
    const double rho   = 28.0;
    const double beta  = 8.0 / 3.0;

    dydt[0] = sigma * (y[1] - y[0]);
    dydt[1] = y[0] * (rho - y[2]) - y[1];
    dydt[2] = y[0] * y[1] - beta * y[2];
}

void rk4_step(double t, double h, double y[N],
              void (*rhs)(double, const double[], double[]))
{
    double k1[N], k2[N], k3[N], k4[N];
    double y_temp[N];
    int i;

    rhs(t, y, k1);
    for (i = 0; i < N; i++) y_temp[i] = y[i] + 0.5 * h * k1[i];
    rhs(t + 0.5 * h, y_temp, k2);
    for (i = 0; i < N; i++) y_temp[i] = y[i] + 0.5 * h * k2[i];
    rhs(t + 0.5 * h, y_temp, k3);
    for (i = 0; i < N; i++) y_temp[i] = y[i] + h * k3[i];
    rhs(t + h, y_temp, k4);
    for (i = 0; i < N; i++) y[i] = y[i] + (h / 6.0) * (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]);
}

void solve_and_write(FILE *fp, const double y0[N], double h, double t1, double t2,
                     void (*rhs)(double, const double[], double[]))
{
    int i, n;
    int steps = (int)((t2 - t1) / h) + 1;
    double y[N];
    double t = t1;

    for (i = 0; i < N; i++) y[i] = y0[i];

    for (n = 0; n < steps; n++) {
        fprintf(fp, "%.4f,%.8f,%.8f,%.8f\n", t, y[0], y[1], y[2]); //write to file
        if (n < steps - 1) {
            rk4_step(t, h, y, rhs);
            t += h;
        }
    }
}

int main(int argc, char *argv[])
{
    double y0[N] = {1.0, 1.0, 1.0};
    double h = 0.01;
    double t1 = 0.0, t2 = 50.0;
    char *filename = "results.csv";

    if (argc > 1) h = atof(argv[1]);
    if (argc > 2) filename = argv[2];

    FILE *fp = fopen(filename, "w");
    if (!fp) return 1;
    
    fprintf(fp, "t,x,y,z\n"); // header in file
    solve_and_write(fp, y0, h, t1, t2, f);
    printf("Successfully wrote results to %s\n", filename);
    fclose(fp);
    return 0;
}

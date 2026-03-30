/*
 *  Assignment2.c
 *  Written by: Harry McKinney Student #: 25258748
 *  Mar 14, 2026
 *  Assignment 2
 */

#include <stdio.h>
#include <math.h>

void exercise1(void); 
void exercise2(void); 
void exercise3(void); 
void exercise4(void); 
double exercise5b(double, double, int);
double exercise5c(double, double, int);
void exercise5d(void); 
void exercise6(double); 
void exercise7(void);

void format_out(int i) {
    printf("\n------------------------------\n");
    printf("Exercise %d\n", i);
    printf("------------------------------\n");
}

int main(void)
{
    format_out(1);
    exercise1();

    format_out(2);
    exercise2();

    format_out(3);
    exercise3();

    format_out(4);
    exercise4();

    format_out(5);
    exercise5b(0.1, 25.0, 1);
    exercise5c(0.1, 25.0, 1);
    exercise5d();

    format_out(6);
    exercise6(0.001);

    format_out(7);
    exercise7();

    return 0;
}

void exercise1(void) {
    double x, y;

    printf("Enter a value for x: ");

    while (scanf("%lf", &x) != 1 || x == -1) {
        printf("Invalid input. Enter a different value for x: ");
        while (getchar() != '\n');
    }

    y = sin(x) / (1.0 + x);
    printf("sin(x)/(1.0+x) = %lf\n", y);

    return;
}

int int_factorial(int n) {
    int result = 1;

    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

unsigned long long factorial(int n) {
    unsigned long long result = 1;

    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

void exercise2(void) {
    int n;

    printf("Enter an integer value for n: ");

    while (scanf("%d", &n) != 1 || n < 0) {
        printf("Invalid input. Enter a different value for n: ");
        while (getchar() != '\n');
    }

    printf("Using int for factorial function: %d! = %d\n", n, int_factorial(n));
    printf("Using unsigned long long for factorial function: %d! = %llu\n", n, factorial(n));

    return;
}

void exercise3(void) {
    int N = 50;
    double x = -5.5;
    double actual = exp(x);
    double sum = 1.0;
    double term = 1.0;
    double error;

    FILE *f = fopen("exercise3_convergence.txt", "w");
    fprintf(f, "i,approximation,error_percent\n");

    for (int i = 1; i <= N; i++){
        term *= x/i;
        sum += term;
        error = fabs((sum - actual) / actual) * 100.0;
        fprintf(f, "%d,%.10f,%.10f\n", i, sum, error);

        if (error < 1.0) {
            printf("Converged to 1%% accuracy at N = %d\n", i);
            break;
        }
    }

    fclose(f);
    printf("Approximation of e^-5.5 = %f\n", sum);
    printf("Actual value of e^-5.5 = %f\n", actual);
}

void exercise4(void) {
    int N = 50;
    double x = 5.5;
    double actual = exp(-1.0*x);
    double sum = 1.0;
    double term = 1.0;
    double error;

    FILE *f = fopen("exercise4_convergence.txt", "w");
    fprintf(f, "i,approximation,error_percent\n");

    for (int i = 1; i <= N; i++){
        term *= x/i;
        sum += term;
        error = fabs((1.0/sum - actual) / actual) * 100.0;
        fprintf(f, "%d,%f,%f\n", i, 1.0/sum, error);

        if (error < 1.0) {
            printf("Converged to 1%% accuracy at N = %d\n", i);
            break;
        }
    }

    fclose(f);
    printf("Approximation of e^-5.5 = %f\n", 1.0/sum);
    printf("Actual value of e^-5.5 = %f\n", actual);
}

double exercise5b(double dt, double time, int writefile) {
    double tau = 5.0;
    double N = 100.0;
    double N_exact, error;
    FILE *f = NULL;

    int steps = (int)(time / dt);

    if (writefile) {
    f = fopen("exercise5_first_order.txt", "w");
    fprintf(f, "time,approximation,exact\n");
    }

    for (int i = 0; i < steps; i++) {
        N = N * (1.0 - dt / tau);
        N_exact = 100.0 * exp(-1.0 * dt * (i+1) / tau);
        if (writefile) fprintf(f, "%f,%f,%f\n", (i+1)*dt, N, N_exact);
    }

    error = fabs((N - N_exact) / N_exact) * 100.0;

    if (writefile) {
        printf("Wrote to first order file.\n");
        fclose(f);
    }


    return error;

}

double exercise5c(double dt, double time, int writefile) {
    double tau = 5.0;
    double N_prev = 100.0;
    double N_curr = N_prev - (dt / tau) * N_prev;  
    double N_exact, error;
    FILE *f = NULL;

    int steps = (int)(time / dt);

    if (writefile) {
    f = fopen("exercise5_leapfrog.txt", "w");
    fprintf(f, "time,approximation,exact\n");
    fprintf(f, "0.0,%f,%f\n", N_prev, 100.0);
    N_exact = 100.0 * exp(-dt / tau);
    fprintf(f, "%f,%f,%f\n", dt, N_curr, N_exact);
    }

    for (int i = 1; i < steps; i++) {
        double N_next = N_prev - (2.0 * dt / tau) * N_curr;

        N_exact = 100.0 * exp(-(i + 1) * dt / tau);
        if (writefile) fprintf(f, "%f,%f,%f\n", (i + 1) * dt, N_next, N_exact);

        N_prev = N_curr;
        N_curr = N_next;
    }

    error = fabs((N_curr - N_exact) / N_exact) * 100.0;

     if (writefile) {
        printf("Wrote to leapfrog file.\n");
        fclose(f);
    }

    return error;

}

void exercise5d(void) {
    int num_dt = 100;
    FILE *f = fopen("exercise5d_errors.txt", "w");
    fprintf(f, "dt,error_euler,error_leapfrog\n");

    for (int k = 0; k < num_dt; k++) {
        double dt = pow(10.0, -6.0 + 6.0 * k / (num_dt - 1));
        fprintf(f, "%.10f,%.10f,%.10f\n", dt, exercise5b(dt, 5.0, 0), exercise5c(dt, 5.0, 0));
    }

    printf("Wrote to errors file.\n");
    fclose(f);

}

double f(double x) {
    return pow(x, 3.0) - 2.2*x;
}

double g(double x) {
    return f(f(f(f(x))));
}

void exercise6(double eps) {
    double a = 1.0;
    double b = 1.6;
    double c = 0.5*(a+b);
    int i;

    if (g(a)*g(b) > 0) {
        printf("Initial guess will not find the roots.");
    }
    else {
        while (fabs(g(c)) > eps && fabs(b-a) > eps) {
            c = 0.5*(a+b);
            if (g(a)*g(c) < 0) {
                b = c;
            }
            else {
                a = c;
            }
        }
        printf("There is a root at %f\n", c);

    }

    return;
}

double integrand(double x) {
    return 1.0 / sqrt(x*x + 1.0);
}

void exercise7(void) {
    double a = 0.0; 
    double b = 2.0;  
    int n = 100;     
    
    double h = (b - a) / n;
    double sum = integrand(a) + integrand(b); // sum start and end 

    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        if (i % 2 == 0) {
            sum += 2.0 * integrand(x); //even
        } else {
            sum += 4.0 * integrand(x); //odd
        }
    }

    double simpsons_result = (h / 3.0) * sum;
    double exact_result = log(2.0 + sqrt(5.0));
    double diff = fabs(simpsons_result - exact_result);

    printf("Exact Value (Factor of k*lambda): %.10f\n", exact_result);
    printf("Simpson's Rule Result:            %.10f\n", simpsons_result);
    printf("Absolute Difference:              %e\n", diff);

}
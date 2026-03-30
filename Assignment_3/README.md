# SHPC4001 Assignment 3: Numerical Solutions to Differential Equations

This repository contains numerical solvers for the Lorenz system, the Time-Independent Schrödinger Equation (TISE), and matrix eigenvalue problems.

## Files
- `lorenz_system.c`: C implementation of the Lorenz system using the 4th-order Runge-Kutta (RK4) method.
- `schrodinger.c`: C implementation of a shooting method using the Numerov-like recurrence to find eigenvalues and eigenfunctions of the 1D quantum harmonic oscillator.
- `matrix_solver.c`: C implementation using LAPACKE to solve the eigensystem of a symmetric tridiagonal banded matrix.
- `inverse_power.c`: C implementation of the normalized inverse power iteration method for 3x3 complex matrices.

## Compilation and Execution

### Lorenz System
To compile the Lorenz system:
```bash
gcc lorenz_system.c -o lorenz
```
To run the simulation:
```bash
./lorenz [h] [output_filename]
```
This generates `results.csv`, or whatever you named the file.

**Manual Configuration:**
The time step `h` can be adjusted by passing it as the first command-line argument. If not provided, it defaults to `0.01`. Other parameters are hardcoded.

### Schrödinger Equation
To compile the Schrödinger solver:
```bash
gcc schrodinger.c -o schrodinger
```
To run the solver:
```bash
./schrodinger
```
**Manual Configuration:**
The grid size `N` and the boundary limit `L` are defined as macros at the top of `schrodinger.c`. The step size `h` is derived from these values:
```c
#define N 1000
#define L 5.0
#define h (2.0*L/N)
```
To change `L` or the step size `h` (via `N`), you must manually edit these lines in the source code and recompile the program.

### Inverse Power Method
To compile the inverse power method:
```bash
gcc inverse_power.c -o inverse_power 
```
To run the iteration:
```bash
./inverse_power
```
This generates `convergence.csv`.

### Matrix Solver (LAPACKE)
To compile the matrix solver (requires LAPACKE):
```bash
gcc matrix_solver.c -o matrix_solver -llapacke -llapack -lblas
```
Example to run the solver for a matrix of dimension N=100:
```bash
./matrix_solver 100
```
This generates `eigenvalues.csv` and `eigenvectors.csv`.

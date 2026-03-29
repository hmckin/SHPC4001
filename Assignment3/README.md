# SHPC4001 Assignment 3: Numerical Solutions to Differential Equations

This repository contains numerical solvers for the Lorenz system and the Time-Independent Schrödinger Equation (TISE) for a quantum harmonic oscillator.

## Files
- `lorenz_system.c`: C implementation of the Lorenz system using the 4th-order Runge-Kutta (RK4) method.
- `schrodinger.c`: C implementation of a shooting method using the Numerov-like recurrence to find eigenvalues and eigenfunctions of the 1D quantum harmonic oscillator.
- `plot_results.py`: Python script for plotting the Lorenz system results (3D and subplots).
- `plot_schrodinger.py`: Python script for plotting the wavefunctions and checking convergence of the Schrödinger solver.

## Compilation and Execution

### Lorenz System
To compile the Lorenz system:
```bash
gcc -O3 lorenz_system.c -o lorenz -lm
```
To run the simulation:
```bash
./lorenz [h] [output_filename]
```
**Manual Configuration:**
The time step `h` can be adjusted by passing it as the first command-line argument. If not provided, it defaults to `0.01`. Other parameters like initial conditions (`y0`) and simulation time (`t1`, `t2`) are currently hardcoded in the `main` function of `lorenz_system.c`.

### Schrödinger Equation
To compile the Schrödinger solver:
```bash
gcc -O3 schrodinger.c -o schrodinger -lm
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
To change `L` or the step size `h` (via `N`), you must manually edit these `#define` lines in the source code and recompile the program.

## Plotting Results
The Python scripts require `numpy`, `matplotlib`, and `pandas`.
```bash
python3 plot_results.py
python3 plot_schrodinger.py
```

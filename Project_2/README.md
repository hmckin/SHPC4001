# SHPC4001 Project 2: N-Body Simulation with OpenMP

Written by: Harry McKinney (Student #: 25258748)
May, 2026

N-body gravitational simulation comparing serial and OpenMP-parallelized implementations, including runtime and scaling analysis.

## Files
- `01_nbody_cpu_serial.c`: Updated serial implementation of the n-body simulation.
- `01_nbody_cpu_serial_orig.c`: Original serial implementation (baseline).
- `02_nbody_cpu_openmp_loop.c`: OpenMP version using loop parallelism pragmas across acceleration, velocity, and position.
- `README.md`: Documentation for the n-body project and scaling tests.

## Compilation and Execution
Build with the included Makefile:
```bash
make
```
Run the serial version:
```bash
./bin/01_nbody_cpu_serial_c
```
Run the OpenMP version (adjust the thread count as needed):
```bash
OMP_NUM_THREADS=4 ./bin/02_nbody_cpu_openmp_loop_c
```
Output files and scaling results are written to the `scaling_results/` directory.

## Scaling Analysis
The assignment includes strong scaling tests across particle counts (N) and thread counts (P), with runtime/speedup plots produced by the plotting scripts.
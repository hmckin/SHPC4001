# SHPC4001 Assignment 4: Fourier Analysis and Monte Carlo Integration
Written by: Harry McKinney 25258748
Date: April 22nd 2026

This repository contains numerical implementations for Fourier series expansions, Discrete Fourier Transforms (DFT), quantum wavepacket evolution using FFTs, and Monte Carlo integration.

## Files
- `fourier_dft.c`: C implementation of Fourier series expansions (triangle and sawtooth waves), Lanczos sigma factors, and a naive Discrete Fourier Transform (DFT/IDFT).
- `fft_packet.c`: C implementation using the FFTW library to simulate the evolution of a Gaussian wavepacket in position and momentum space.
- `monte_carlo.c`: C implementation of 3D Monte Carlo integration (Method 1) to evaluate triple integrals.

## Compilation and Execution

### Fourier Series and DFT
To compile the Fourier and DFT solvers:
```bash
gcc fourier_dft.c -o fourier
```
To run the analysis:
```bash
./fourier
```
This generates several files in a `csv/` directory (make one prior to running), including `fourier_sums.csv`, `fourier_convergence.csv`, `fourier_lanczos.csv`, and DFT results for N=32 and N=128.

### Quantum Wavepacket (FFTW)
To compile the wavepacket simulator (requires FFTW3):
```bash
gcc fft_packet.c -o fft -lfftw3 -lm
```
*Note: You may need to specify the include and library paths for FFTW (e.g., `-I/usr/local/include -L/usr/local/lib`).*

To run the simulation:
```bash
./fft
```
This generates `wavepacket_t0.csv`, `wavepacket_t1.csv`, and `wavepacket_t5.csv` in the `csv/` directory.

### Monte Carlo Integration
To compile the Monte Carlo solver:
```bash
gcc monte_carlo.c -o monte_carlo -lm
```
To run the integration:
```bash
./monte_carlo
```
This calculates the convergence against the exact analytical value and generates `csv/monte_carlo_results.csv`.

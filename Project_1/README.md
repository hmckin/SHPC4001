# SHPC4001 Project 1: Distributed Monte Carlo Integration with MPI

Written by: Harry McKinney (Student #: 25258748)
May 4, 2026

Implements a 3D Monte Carlo integration (Method 1) using message passing with MPI, and analyzes strong scaling across process counts.

## Files
- `monte_carlo_original.c`: Serial baseline version of the 3D Monte Carlo integration.
- `monte_carlo_mpi.c`: MPI-parallelized version that distributes samples across processes.
- `compare_performance.py`: Parses benchmark output and plots runtime/speedup results.
- `job.slurm`: Slurm batch script to run the strong scaling benchmark on Setonix.

## Compilation and Execution
Compile the MPI version (requires an MPI compiler):
```bash
mpicc monte_carlo_mpi.c -o monteruntime -lm
```
Run with a chosen number of processes:
```bash
mpirun -np 4 ./monteruntime
```
To run the full strong-scaling benchmark on Slurm:
```bash
sbatch job.slurm
```
After collecting output, generate the performance comparison:
```bash
python3 compare_performance.py
```
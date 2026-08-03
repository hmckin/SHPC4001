#!/bin/bash --login
#SBATCH --partition=work
#SBATCH --ntasks=8
#SBATCH --ntasks-per-node=8
#SBATCH --sockets-per-node=1
#SBATCH --time=00:10:00
#SBATCH --output=benchmark.out

echo "Monte Carlo Strong Scaling Benchmark"
echo "--------------------------------------------------------"

for nprocs in 1 2 4 8
do
    echo "Running with $nprocs processes..."
    srun -N 1 -n $nprocs -c 1 ./monte_carlo_mpi
    echo "--------------------------------------------------------"
done

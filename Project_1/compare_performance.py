import pandas as pd
import re
import os
import matplotlib.pyplot as plt

def parse_mpi_results(filename):
    """Parses benchmark.out for all N and runtimes per process count."""
    results = {}
    if not os.path.exists(filename):
        return results

    with open(filename, 'r') as f:
        content = f.read()

    sections = re.split(r'Running with (\d+) processes...', content)
    
    for i in range(1, len(sections), 2):
        nprocs = int(sections[i])
        data = sections[i+1]
        
        matches = re.findall(r'N =\s+(\d+)\s+\|\s+Time:\s+([\d.]+)\s+s', data)
        if matches:
            results[nprocs] = {
                'N': [int(m[0]) for m in matches],
                'Time': [float(m[1]) for m in matches]
            }
    
    return results

def plot_comparison():
    csv_path = 'csv/monte_carlo_results.csv'
    out_path = 'benchmark.out'

    plt.figure(figsize=(10, 6))

    # 1. Plot Serial Data
    try:
        df_serial = pd.read_csv(csv_path)
        plt.plot(df_serial['N'], df_serial['runtime_s'], 'k--', label='Serial (Original)', linewidth=2)
    except Exception as e:
        print(f"Warning: Could not read serial CSV: {e}")

    # 2. Plot MPI Data
    mpi_data = parse_mpi_results(out_path)
    colors = {1: 'blue', 2: 'green', 4: 'orange', 8: 'red'}
    
    for procs in sorted(mpi_data.keys()):
        data = mpi_data[procs]
        plt.plot(data['N'], data['Time'], label=f'MPI ({procs} procs)', color=colors.get(procs), marker='o', markersize=3)

    # Formatting
    plt.xscale('log')
    plt.yscale('log')
    plt.xlabel('Number of Samples (N)')
    plt.ylabel('Runtime (seconds)')
    plt.title('Monte Carlo Integration: Performance Scaling')
    plt.grid(True, which="both", ls="-", alpha=0.5)
    plt.legend()
    
    # Save the plot
    plot_file = 'performance_graph.png'
    plt.savefig(plot_file)
    print(f"Graph saved as: {plot_file}")
    
    # 3. Print a summary table for the largest N
    print("\nSummary at Largest N:")
    print(f"{'Config':<12} | {'Procs':<6} | {'Time (s)':<10}")
    print("-" * 35)
    
    for procs in sorted(mpi_data.keys()):
        t = mpi_data[procs]['Time'][-1]
        print(f"{'MPI':<12} | {procs:<6} | {t:<10.4f}")

if __name__ == "__main__":
    plot_comparison()

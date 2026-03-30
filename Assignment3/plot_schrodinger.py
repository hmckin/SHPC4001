import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import glob
import re

def parse_header_energies(filepath):
    with open(filepath, 'r') as f:
        line = f.readline()
        if line.startswith('#'):
            # Extract energies using regex: E0:0.5000, E1:1.5000...
            energies = re.findall(r'E\d+:([\d\.]+)', line)
            return [float(e) for e in energies]
    return None

def plot_convergence():
    files = glob.glob('schrodinger_results_*.csv')
    
    h_data = []
    L_data = []
    
    for f in files:
        # Try to extract h and L from filename
        # Pattern: schrodinger_results_h0.0100.csv or schrodinger_results_L5.000.csv
        # Using a more precise regex to avoid capturing the trailing dot before .csv
        h_match = re.search(r'_h(\d+\.?\d*)', f)
        L_match = re.search(r'_L(\d+\.?\d*)', f)
        
        energies = parse_header_energies(f)
        if energies is None:
            continue
            
        theory = np.array([0.5, 1.5, 2.5, 3.5])
        errors = np.abs(np.array(energies) - theory)
        # We'll take the ground state (E0) for clarity.
        error_val = errors[0] 
        
        if h_match:
            h_data.append((float(h_match.group(1)), error_val))
        if L_match:
            L_data.append((float(L_match.group(1)), error_val))

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))

    if h_data:
        h_data.sort()
        hs, h_errs = zip(*h_data)
        ax1.loglog(hs, h_errs, 'o-', label='Numerical Error ($E_0$)')
        # Add a reference line for O(h^2) convergence
        ax1.loglog(hs, [h**2 * (h_errs[0]/hs[0]**2) for h in hs], '--', alpha=0.5, label='$O(h^2)$')
        ax1.set_xlabel('Step size $h$')
        ax1.set_ylabel('Absolute Error $|E_{num} - E_{theory}|$')
        ax1.set_title('Convergence vs. Step Size ($h$)')
        ax1.legend()
        ax1.grid(True, which="both", ls="-", alpha=0.5)

    if L_data:
        L_data.sort()
        Ls, L_errs = zip(*L_data)
        ax2.loglog(Ls, L_errs, 's-', color='orange', label='Numerical Error ($E_0$)')
        ax2.set_xlabel('Domain Size $L$')
        ax2.set_ylabel('Absolute Error $|E_{num} - E_{theory}|$')
        ax2.set_title('Convergence vs. Domain Size ($L$)')
        ax2.legend()
        ax2.grid(True, which="both", ls="-", alpha=0.5)

    plt.tight_layout()
    plt.savefig('schrodinger_convergence.png')
    print("Convergence plots saved as schrodinger_convergence.png")

    # Also keep the original wavefunction plotting logic if requested
    # (Just plotting the most recent/default file)
    df = pd.read_csv('schrodinger_results.csv')
    plt.figure(figsize=(10, 6))
    for i in range(4):
        plt.plot(df['x'], df[f'psi{i}'], label=f'State {i}')
    plt.xlabel('$x$')
    plt.ylabel('Wavefunction $\psi(x)$')
    plt.title(f'Lowest 4 Energy States of the 1D Harmonic Oscillator')
    plt.legend()
    plt.grid(True)
    plt.savefig('schrodinger_wavefunctions.png')
    print("Wavefunction plot saved as schrodinger_wavefunctions.png")

if __name__ == "__main__":
    plot_convergence()
